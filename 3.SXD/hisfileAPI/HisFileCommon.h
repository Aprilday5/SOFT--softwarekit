//
// Created by dwolf on 2020/2/8.
//

#ifndef HWG_HISFILECOMMON_H
#define HWG_HISFILECOMMON_H

#include <iostream>
#include <string>
using namespace std;

#define FILE_TYPE_SOE 0
#define FILE_TYPE_CO 1
#define FILE_TYPE_LOG 2

#define u_int8_t unsigned short
#define u_int16_t unsigned int	
#define u_int64_t unsigned long

enum FILE_TYPE{
    FT_SOE =0, // SOE事件记录
    FT_CO, //遥控操作记录
    FT_LOG //日志
};

struct CMD
{
    u_int64_t  info_addr;
    char* tm;
    char* cmd;
    u_int8_t value;
};

struct CMDS
{
    u_int16_t num;
    struct CMD cmd[1024];
};

struct EVENT
{
    u_int16_t  info_addr;
    char* tm;
    u_int8_t value;
};

struct EVENTS
{
    u_int16_t num;
    struct EVENT event[1024];
};

struct LOG
{
    unsigned short log_type;
    char* tm;
    char* text;
    unsigned short value;
};

struct LOGS
{
    u_int16_t num;
    struct LOG log[1024];
};


struct HDB_RECORD
{
    enum FILE_TYPE  file_type;                           //文件格式
    //0（FILE_TYPE_SOE）SOE事件记录
    //1（FILE_TYPE_CO）遥控操作记录
    //2（FILE_TYPE_LOG）日志
    union
    {
        struct EVENTS      events;             //事件记录
        struct CMDS        cmds;               //操作记录
        struct LOGS        logs;               //日志
    }record;
};



#endif //HWG_HISFILECOMMON_H
