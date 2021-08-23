#ifndef AML_IPC_LOG_H

#define AML_IPC_LOG_H

#include <alloca.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#define _GNU_SOURCE
#include <sys/syscall.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AML_LOG_LEVEL_INVALID 0x7fffffff

#define AML_LOG_GET_CAT(name) AML_LOG_##name
#define AML_LOG_DEFINE(name)                                                    \
    struct AmlIPCLogCat AML_LOG_GET_CAT(name) = { #name, AML_LOG_LEVEL_INVALID, \
        AML_LOG_LEVEL_INVALID, NULL }

#define AML_LOG_EXTERN(name) extern struct AmlIPCLogCat AML_LOG_GET_CAT(name)
extern struct AmlIPCLogCat AML_LOG_DEFAULT;

struct AmlIPCLogCat {
    const char* name;
    int log_level;
    int trace_level;
    struct AmlIPCLogCat* next;
};

#define AML_LOG_CAT_ENABLE(cat, l) ((cat)->log_level > l)

#define AML_LOG_GET_CAT_(name) AML_LOG_##name
#define AML_LOG_CAT(cat, level, fmt, ...)                                      \
    do {                                                                       \
        if (AML_LOG_CAT_ENABLE(&(AML_LOG_GET_CAT_(cat)), level)) {             \
            aml_ipc_log_msg(&(AML_LOG_GET_CAT_(cat)),                          \
                syscall(SYS_gettid), level, __FILE__, __func__, __LINE__, fmt, \
                ##__VA_ARGS__);                                                \
        }                                                                      \
    } while (0)

#define AML_LOGVVV(fmt, ...) AML_LOG_CAT(DEFAULT, 6, fmt, ##__VA_ARGS__)
#define AML_LOGVV(fmt, ...) AML_LOG_CAT(DEFAULT, 5, fmt, ##__VA_ARGS__)
#define AML_LOGV(fmt, ...) AML_LOG_CAT(DEFAULT, 4, fmt, ##__VA_ARGS__)
#define AML_LOGD(fmt, ...) AML_LOG_CAT(DEFAULT, 3, fmt, ##__VA_ARGS__)
#define AML_LOGI(fmt, ...) AML_LOG_CAT(DEFAULT, 2, fmt, ##__VA_ARGS__)
#define AML_LOGW(fmt, ...) AML_LOG_CAT(DEFAULT, 1, fmt, ##__VA_ARGS__)
#define AML_LOGE(fmt, ...) AML_LOG_CAT(DEFAULT, 0, fmt, ##__VA_ARGS__)

#define AML_LOGCATVVV(cat, fmt, ...) AML_LOG_CAT(cat, 6, fmt, ##__VA_ARGS__)
#define AML_LOGCATVV(cat, fmt, ...) AML_LOG_CAT(cat, 5, fmt, ##__VA_ARGS__)
#define AML_LOGCATV(cat, fmt, ...) AML_LOG_CAT(cat, 4, fmt, ##__VA_ARGS__)
#define AML_LOGCATD(cat, fmt, ...) AML_LOG_CAT(cat, 3, fmt, ##__VA_ARGS__)
#define AML_LOGCATI(cat, fmt, ...) AML_LOG_CAT(cat, 2, fmt, ##__VA_ARGS__)
#define AML_LOGCATW(cat, fmt, ...) AML_LOG_CAT(cat, 1, fmt, ##__VA_ARGS__)
#define AML_LOGCATE(cat, fmt, ...) AML_LOG_CAT(cat, 0, fmt, ##__VA_ARGS__)

// log the time spend in current scope
struct AmlIPCScopeTimer {
    char buf[256];
    struct timespec start_time;
    struct timespec start_ttime;
    struct AmlIPCLogCat* cat;
    const char* file;
    const char* func;
    int level;
    int line;
};
extern __thread int sc_level__;
void stop_scope_timer__(struct AmlIPCScopeTimer** timer);

#define AML_TRACE_CAT_ENABLE(cat, l) ((cat)->trace_level > l)
#define AML_SCTIME_LOGCAT_(_id, _cat, _level, _fmt, ...)                              \
    __attribute__((cleanup(stop_scope_timer__))) struct AmlIPCScopeTimer* _id = NULL; \
    do {                                                                              \
        ++sc_level__;                                                                 \
        if (AML_TRACE_CAT_ENABLE(&(_cat), _level)) {                                  \
            _id = (typeof(_id) alloca(sizeof(*_id)));                                 \
            _id->cat = &_cat;                                                         \
            _id->level = _level;                                                      \
            _id->file = __FILE__;                                                     \
            _id->func = __func__;                                                     \
            _id->line = __LINE__;                                                     \
            clock_gettime(CLOCK_MONOTONIC, &_id->start_time);                         \
            clock_gettime(CLOCK_THREAD_CPUTIME_ID, &_id->start_time);                 \
            snprintf(_id->buf, sizeof(_id->buf), _fmt, ##__VA_ARGS__);                \
        }                                                                             \
    } while (0)

#define _MCAT_(x, y) x##y
#define _MCAT_2_(x, y) _MCAT_(x, y)
#define AML_SCTIME_LOG_CAT(cat, level, fmt, ...)                                           \
    AML_SCTIME_LOGCAT_(_MCAT_2_(_sctime_, __COUNTER__), AML_LOG_GET_CAT_(cat), level, fmt, \
        ##__VA_ARGS__)

#define AML_SCTIME_LOGVV(fmt, ...) AML_SCTIME_CAT(DEFAULT, 5, fmt, ##__VA_ARGS__)
#define AML_SCTIME_LOGV(fmt, ...) AML_SCTIME_CAT(DEFAULT, 4, fmt, ##__VA_ARGS__)
#define AML_SCTIME_LOGD(fmt, ...) AML_SCTIME_CAT(DEFAULT, 3, fmt, ##__VA_ARGS__)
#define AML_SCTIME_LOGI(fmt, ...) AML_SCTIME_CAT(DEFAULT, 2, fmt, ##__VA_ARGS__)
#define AML_SCTIME_LOGW(fmt, ...) AML_SCTIME_CAT(DEFAULT, 1, fmt, ##__VA_ARGS__)
#define AML_SCTIME_LOGE(fmt, ...) AML_SCTIME_CAT(DEFAULT, 0, fmt, ##__VA_ARGS__)

#define AML_SCTIME_LOGCATVV(cat, fmt, ...) AML_SCTIME_LOG_CAT(cat, 5, fmt, ##__VA_ARGS__)
#define AML_SCTIME_LOGCATV(cat, fmt, ...) AML_SCTIME_LOG_CAT(cat, 4, fmt, ##__VA_ARGS__)
#define AML_SCTIME_LOGCATD(cat, fmt, ...) AML_SCTIME_LOG_CAT(cat, 3, fmt, ##__VA_ARGS__)
#define AML_SCTIME_LOGCATI(cat, fmt, ...) AML_SCTIME_LOG_CAT(cat, 2, fmt, ##__VA_ARGS__)
#define AML_SCTIME_LOGCATW(cat, fmt, ...) AML_SCTIME_LOG_CAT(cat, 1, fmt, ##__VA_ARGS__)
#define AML_SCTIME_LOGCATE(cat, fmt, ...) AML_SCTIME_LOG_CAT(cat, 0, fmt, ##__VA_ARGS__)

#define AML_TRACE_EVENT_CAT(c, n, ph, id, _cat, _level, _fmt, ...)                   \
    do {                                                                             \
        if (AML_TRACE_CAT_ENABLE(&(AML_LOG_GET_CAT_(_cat)), _level)) {               \
            aml_ipc_trace_log(&(AML_LOG_GET_CAT_(_cat), _level, c, n, ph, id, _fmt)) \
        }                                                                            \
    } while (0)

#define AML_FLOW_START_TRACECATV(cat, c, n, id, fmt, ...) \
    AML_TRACE_EVENT_CAT(c, n, 's', id, cat, 4, fmt, ##__VA_ARGS__)
#define AML_FLOW_START_TRACECATD(cat, c, n, id, fmt, ...) \
    AML_TRACE_EVENT_CAT(c, n, 's', id, cat, 3, fmt, ##__VA_ARGS__)

#define AML_FLOW_STEP_TRACECATV(cat, c, n, id, fmt, ...) \
    AML_TRACE_EVENT_CAT(c, n, 't', id, cat, 4, fmt, ##__VA_ARGS__)
#define AML_FLOW_STEP_TRACECATD(cat, c, n, id, fmt, ...) \
    AML_TRACE_EVENT_CAT(c, n, 't', id, cat, 3, fmt, ##__VA_ARGS__)

#define AML_FLOW_END_TRACECATV(cat, c, n, id, fmt, ...) \
    AML_TRACE_EVENT_CAT(c, n, 'f', id, cat, 4, fmt, ##__VA_ARGS__)
#define AML_FLOW_END_TRACECATD(cat, c, n, id, fmt, ...) \
    AML_TRACE_EVENT_CAT(c, n, 'f', id, cat, 3, fmt, ##__VA_ARGS__)

#define AML_ASYNC_START_TRACECATV(cat, c, n, id, fmt, ...) \
    AML_TRACE_EVENT_CAT(c, n, 's', id, cat, 4, fmt, ##__VA_ARGS__)
#define AML_ASYNC_START_TRACECATD(cat, c, n, id, fmt, ...) \
    AML_TRACE_EVENT_CAT(c, n, 's', id, cat, 3, fmt, ##__VA_ARGS__)

#define AML_ASYNC_STEP_TRACECATV(cat, c, n, id, fmt, ...) \
    AML_TRACE_EVENT_CAT(c, n, 't', id, cat, 4, fmt, ##__VA_ARGS__)
#define AML_ASYNC_STEP_TRACECATD(cat, c, n, id, fmt, ...) \
    AML_TRACE_EVENT_CAT(c, n, 't', id, cat, 3, fmt, ##__VA_ARGS__)

#define AML_ASYNC_END_TRACECATV(cat, c, n, id, fmt, ...) \
    AML_TRACE_EVENT_CAT(c, n, 'f', id, cat, 4, fmt, ##__VA_ARGS__)
#define AML_ASYNC_END_TRACECATD(cat, c, n, id, fmt, ...) \
    AML_TRACE_EVENT_CAT(c, n, 'f', id, cat, 3, fmt, ##__VA_ARGS__)

__attribute__((format(printf, 7, 8))) void aml_ipc_log_msg(struct AmlIPCLogCat* cat, int t, int level, const char* file, const char* function, int lineno, const char* fmt, ...);

/**
* @brief  set log level from a string
*
* @Param str, ',' seperate categories and levels
* cat1:level1,cat2:level2...
*/
void aml_ipc_log_set_from_string(const char* str);
void aml_ipc_trace_set_from_string(const char* str);

void aml_ipc_log_set_output_file(FILE* fp);

void aml_ipc_trace_set_json_output(FILE* fp);
__attribute__((format(printf, 7, 8))) void aml_ipc_trace_log(struct AmlIPCLogCat* cat, int level, const char* evcat, const char* evname, int evph, void* evid, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif // !AML_IPC_LOG_H
