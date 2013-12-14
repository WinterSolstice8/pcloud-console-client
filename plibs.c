#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sqlite3.h>
#include "plibs.h"

const static char *psync_typenames[]={"[invalid type]", "[number]", "[string]", "[float]", "[null]"};

pthread_mutex_t psync_db_mutex;
sqlite3 *psync_db;
pstatus_t psync_status;
int psync_do_run=1;

char *psync_strdup(const char *str){
  size_t len;
  char *ptr;
  len=strlen(str)+1;
  ptr=(char *)psync_malloc(len);
  memcpy(ptr, str, len);
  return ptr;
}

char *psync_strcat(const char *str, ...){
  size_t i, size, len;
  const char *strs[64];
  size_t lengths[64];
  const char *ptr;
  char *ptr2, *ptr3;
  va_list ap;
  va_start(ap, str);
  strs[0]=str;
  len=strlen(str);
  lengths[0]=len;
  size=len+1;
  i=1;
  while ((ptr=va_arg(ap, const char *))){
    len=strlen(ptr);
    lengths[i]=len;
    strs[i++]=ptr;
    size+=len;
  }
  va_end(ap);
  ptr2=ptr3=(char *)psync_malloc(size);
  for (size=0; size<i; size++){
    memcpy(ptr2, strs[size], lengths[size]);
    ptr2+=lengths[size];
  }
  *ptr2=0;
  return ptr3;
}

int psync_sql_connect(const char *db){
  pthread_mutexattr_t mattr;
  int code=sqlite3_open(db, &psync_db);
  if (code==SQLITE_OK){
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&psync_db_mutex, &mattr);
    return 0;
  }
  else{
    debug(D_CRITICAL, "could not open sqlite dabase %s: %s", db, sqlite3_errstr(code));
    return -1;
  }
}

void psync_sql_close(){
  int code=sqlite3_close(psync_db);
  if (code!=SQLITE_OK)
    debug(D_CRITICAL, "error when closing database: %s", sqlite3_errstr(code));
}

void psync_sql_lock(){
  pthread_mutex_lock(&psync_db_mutex);
}

void psync_sql_unlock(){
  pthread_mutex_unlock(&psync_db_mutex);
}

int psync_sql_statement(const char *sql){
  char *errmsg;
  int code;
  psync_sql_lock();
  code=sqlite3_exec(psync_db, sql, NULL, NULL, &errmsg);
  psync_sql_unlock();
  if (code==SQLITE_OK)
    return 0;
  else{
    debug(D_ERROR, "error running sql statement: %s: %s", sql, errmsg);
    sqlite3_free(errmsg);
    return -1;
  }
}

char *psync_sql_cellstr(const char *sql){
  sqlite3_stmt *stmt;
  int code;
  psync_sql_lock();
  code=sqlite3_prepare_v2(psync_db, sql, -1, &stmt, NULL);
  if (code!=SQLITE_OK){
    psync_sql_unlock();
    debug(D_ERROR, "error running sql statement: %s: %s", sql, sqlite3_errstr(code));
    return NULL;
  }
  code=sqlite3_step(stmt);
  if (code==SQLITE_ROW){
    char *ret;
    ret=(char *)sqlite3_column_text(stmt, 0);
    if (ret)
      ret=psync_strdup(ret);
    sqlite3_finalize(stmt);
    psync_sql_unlock();
    return ret;
  }
  else {
    sqlite3_finalize(stmt);
    psync_sql_unlock();
    if (code!=SQLITE_DONE)
      debug(D_ERROR, "sqlite3_step returned error: %s: %s", sql, sqlite3_errstr(code));
    return NULL;
  }
}

int64_t psync_sql_cellint(const char *sql, int64_t dflt){
  sqlite3_stmt *stmt;
  int code;
  psync_sql_lock();
  code=sqlite3_prepare_v2(psync_db, sql, -1, &stmt, NULL);
  if (code!=SQLITE_OK){
    debug(D_ERROR, "error running sql statement: %s: %s", sql, sqlite3_errstr(code));
  }
  else{
    code=sqlite3_step(stmt);
    if (code==SQLITE_ROW)
      dflt=sqlite3_column_int64(stmt, 0);
    else {
      if (code!=SQLITE_DONE)
        debug(D_ERROR, "sqlite3_step returned error: %s: %s", sql, sqlite3_errstr(code));
    }
    sqlite3_finalize(stmt);
  }
  psync_sql_unlock();
  return dflt;
}

char **psync_sql_rowstr(const char *sql){
  sqlite3_stmt *stmt;
  int code, cnt;
  psync_sql_lock();
  code=sqlite3_prepare_v2(psync_db, sql, -1, &stmt, NULL);
  if (code!=SQLITE_OK){
    psync_sql_unlock();
    debug(D_ERROR, "error running sql statement: %s: %s", sql, sqlite3_errstr(code));
    return NULL;
  }
  cnt=sqlite3_column_count(stmt);
  code=sqlite3_step(stmt);
  if (code==SQLITE_ROW){
    char **arr, *nstr, *str;
    size_t l, ln, lens[cnt];
    int i;
    ln=0;
    for (i=0; i<cnt; i++){
      l=sqlite3_column_bytes(stmt, i);
      ln+=l;
      lens[i]=l;
    }
    ln+=(sizeof(char *)+1)*cnt;
    arr=(char **)psync_malloc(ln);
    nstr=((char *)arr)+sizeof(char *)*cnt;
    for (i=0; i<cnt; i++){
      str=(char *)sqlite3_column_blob(stmt, i);
      if (str){
        ln=lens[i];
        memcpy(nstr, str, ln);
        nstr[ln]=0;
        arr[i]=nstr;
        nstr+=ln+1;
      }
      else
        arr[i]=NULL;
    }
    sqlite3_finalize(stmt);
    psync_sql_unlock();
    return arr;
  }
  else {
    sqlite3_finalize(stmt);
    psync_sql_unlock();
    if (code!=SQLITE_DONE)
      debug(D_ERROR, "sqlite3_step returned error: %s: %s", sql, sqlite3_errstr(code));
    return NULL;
  }
}

psync_variant *psync_sql_row(const char *sql){
  sqlite3_stmt *stmt;
  int code, cnt;
  psync_sql_lock();
  code=sqlite3_prepare_v2(psync_db, sql, -1, &stmt, NULL);
  if (code!=SQLITE_OK){
    psync_sql_unlock();
    debug(D_ERROR, "error running sql statement: %s: %s", sql, sqlite3_errstr(code));
    return NULL;
  }
  cnt=sqlite3_column_count(stmt);
  code=sqlite3_step(stmt);
  if (code==SQLITE_ROW){
    psync_variant *arr;
    char *nstr, *str;
    size_t l, ln, lens[cnt];
    int i, t, types[cnt];
    ln=sizeof(psync_variant)*cnt;
    for (i=0; i<cnt; i++){
      t=sqlite3_column_type(stmt, i);
      types[i]=t;
      if (t==SQLITE_TEXT || t==SQLITE_BLOB){
        l=sqlite3_column_bytes(stmt, i);
        ln+=l+1;
        lens[i]=l;
      }
    }
    arr=(psync_variant *)psync_malloc(ln);
    nstr=((char *)arr)+sizeof(psync_variant)*cnt;
    for (i=0; i<cnt; i++){
      t=types[i];
      if (t==SQLITE_INTEGER){
        arr[i].type=PSYNC_TNUMBER;
        arr[i].snum=sqlite3_column_int64(stmt, i);
      }
      else if (t==SQLITE_TEXT || t==SQLITE_BLOB){
        str=(char *)sqlite3_column_blob(stmt, i);
        ln=lens[i];
        memcpy(nstr, str, ln);
        nstr[ln]=0;
        arr[i].type=PSYNC_TSTRING;
        arr[i].str=nstr;
        nstr+=ln+1;
      }
      else if (t==SQLITE_FLOAT){
        arr[i].type=PSYNC_TREAL;
        arr[i].real=sqlite3_column_double(stmt, i);
      }
      else {
        arr[i].type=PSYNC_TNULL;
      }
    }
    sqlite3_finalize(stmt);
    psync_sql_unlock();
    return arr;
  }
  else {
    sqlite3_finalize(stmt);
    psync_sql_unlock();
    if (code!=SQLITE_DONE)
      debug(D_ERROR, "sqlite3_step returned error: %s: %s", sql, sqlite3_errstr(code));
    return NULL;
  }
}

static void time_format(time_t tm, char *result){
  static const char month_names[12][4]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  static const char day_names[7][4] ={"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  struct tm dt;
  int unsigned y;
  gmtime_r(&tm, &dt);
  memcpy(result, day_names[dt.tm_wday], 3);
  result+=3;
  *result++=',';
  *result++=' ';
  *result++=dt.tm_mday/10+'0';
  *result++=dt.tm_mday%10+'0';
  *result++=' ';
  memcpy(result, month_names[dt.tm_mon], 3);
  result+=3;
  *result++=' ';
  y=dt.tm_year+1900;
  *result++='0'+y/1000;
  y=y%1000;
  *result++='0'+y/100;
  y=y%100;
  *result++='0'+y/10;
  y=y%10;
  *result++='0'+y;
  *result++=' ';
  *result++=dt.tm_hour/10+'0';
  *result++=dt.tm_hour%10+'0';
  *result++=':';
  *result++=dt.tm_min/10+'0';
  *result++=dt.tm_min%10+'0';
  *result++=':';
  *result++=dt.tm_sec/10+'0';
  *result++=dt.tm_sec%10+'0';
  memcpy(result, " +0000", 7); // copies the null byte
}

void psync_debug(const char *file, const char *function, int unsigned line, int unsigned level, const char *fmt, ...){
  static const struct {
    int unsigned level;
    const char *name;
  } debug_levels[]=DEBUG_LEVELS;
  static FILE *log=NULL;
  char dttime[32], format[512];
  va_list ap;
  const char *errname;
  int unsigned i;
  time_t currenttime;
  errname="BAD_ERROR_CODE";
  for (i=0; i<sizeof(debug_levels)/sizeof(debug_levels[0]); i++)
    if (debug_levels[i].level==level){
      errname=debug_levels[i].name;
      break;
    }
  if (!log){
    log=fopen(DEBUG_FILE, "a+");
    if (!log)
      return;
  }
  time(&currenttime);
  time_format(currenttime, dttime);
  snprintf(format, sizeof(format), "%s %s: %s:%u (function %s): %s\n", dttime, errname, file, line, function, fmt);
  format[sizeof(format)-1]=0;
  va_start(ap, fmt);
  vfprintf(log, format, ap);
  va_end(ap);
  fflush(log);
}

static const char *get_type_name(uint32_t t){
  if (t>=sizeof(psync_typenames)/sizeof(const char *))
    t=0;
  return psync_typenames[t];
}

uint64_t psync_err_number_expected(const char *file, const char *function, int unsigned line, psync_variant *v){
  if (D_ERROR<=DEBUG_LEVEL)
    psync_debug(file, function, line, D_ERROR, "type error, wanted %s got %s", get_type_name(PSYNC_TNUMBER), get_type_name(v->type));
  return 0;
}

const char *psync_err_string_expected(const char *file, const char *function, int unsigned line, psync_variant *v){
  if (D_ERROR<=DEBUG_LEVEL)
    psync_debug(file, function, line, D_ERROR, "type error, wanted %s got %s", get_type_name(PSYNC_TSTRING), get_type_name(v->type));
  return "";
}

const char *psync_err_lstring_expected(const char *file, const char *function, int unsigned line, psync_variant *v, size_t *len){
  if (D_ERROR<=DEBUG_LEVEL)
    psync_debug(file, function, line, D_ERROR, "type error, wanted %s got %s", get_type_name(PSYNC_TSTRING), get_type_name(v->type));
  *len=0;
  return "";
}

double psync_err_real_expected(const char *file, const char *function, int unsigned line, psync_variant *v){
  if (D_ERROR<=DEBUG_LEVEL)
    psync_debug(file, function, line, D_ERROR, "type error, wanted %s got %s", get_type_name(PSYNC_TREAL), get_type_name(v->type));
  return 0.0;
}
