#include <iostream>
#include "DevHisManage.h"
#include "HisFileCommon.h"


void testSoe(){
    DevHisManage* devHisManage = new DevHisManage(".");
    struct  HDB_RECORD hdbRecord;
    devHisManage->read_record(FT_SOE,&hdbRecord);
    int i = 0;
    int count = hdbRecord.record.events.num+1; //读取所有记录，并打印出来
    for(i = 0; i< count;i++){
        struct  EVENT event = hdbRecord.record.events.event[i];
        cout << "ioa:" <<event.info_addr << " tm:" << event.tm << " value:" << event.value << endl;
    }
    devHisManage = new DevHisManage(".");
    cout << devHisManage->read_record_num(FT_SOE) << endl;
  //  for(int i = 0; i < 10; i++){      //插入若干条记录
    struct  HDB_RECORD addRecord;
    addRecord.file_type = FT_SOE;
    addRecord.record.events.num = 1;
    addRecord.record.events.event[0].info_addr = 900;
    addRecord.record.events.event[0].value = 1;
    addRecord.record.events.event[0].tm = (char *)"bwd";
    devHisManage->insert_record(&addRecord);
//    }

    devHisManage = new DevHisManage(".");
    devHisManage->read_record(FT_SOE,&hdbRecord);
    i = 0;
    count = hdbRecord.record.events.num+1;
    for(i = 0; i< count;i++){
        struct  EVENT event = hdbRecord.record.events.event[i];
        cout << "ioa:" <<event.info_addr << " tm:" << event.tm << " value:" << event.value << endl;
    }
    devHisManage = new DevHisManage(".");
    cout << devHisManage->read_record_num(FT_SOE) << endl;  //读取记录条数
}

void testCMD(){
    DevHisManage* devHisManage = new DevHisManage(".");
    struct  HDB_RECORD hdbRecord;
    int result = devHisManage->read_record(FT_CO,&hdbRecord);
    cout<< "读取结果:"  << result << endl;
    int i = 0;
    int count = 0;
    if(result == -1){
        int count = hdbRecord.record.cmds.num+0;
        for(i = 0; i< count;i++){
            struct  CMD cmd = hdbRecord.record.cmds.cmd[i];
            cout << "ioa:" <<cmd.info_addr << " tm:" << cmd.tm << " value:" << cmd.value << endl;
        }
    }


    devHisManage = new DevHisManage(".");   //插入若干条记录
//    for(int i = 0; i < 1000; i++){
    struct  HDB_RECORD addRecord;
    addRecord.file_type = FT_CO;
    addRecord.record.cmds.num = 1;
    addRecord.record.cmds.cmd[0].info_addr = 900;
    addRecord.record.cmds.cmd[0].value = 1;
    addRecord.record.cmds.cmd[0].tm = (char *)"bwd";
    addRecord.record.cmds.cmd[0].cmd = (char *)"ab";
    result = devHisManage->insert_record(&addRecord);
    cout<< "读取结果:"  << result << endl;
//    }

    devHisManage = new DevHisManage(".");
    devHisManage->read_record(FT_CO,&hdbRecord);//读取所有记录，并打印出来
    i = 0;
    count = hdbRecord.record.cmds.num+1;
    for(i = 0; i< count;i++){
        struct  CMD event = hdbRecord.record.cmds.cmd[i];
        cout << "ioa:" <<event.info_addr << " tm:" << event.tm << " value:" << event.value << endl;
    }
    devHisManage = new DevHisManage(".");
    cout << devHisManage->read_record_num(FT_CO) << endl;//读取记录条数
}

void testLog(){
    DevHisManage* devHisManage = new DevHisManage(".");
    struct  HDB_RECORD hdbRecord;
    int result = devHisManage->read_record(FT_LOG,&hdbRecord);
    cout<< "读取结果:"  << result << endl;
    int i = 0;
    int count = 0;
    if(result == -1){
        int count = hdbRecord.record.logs.num+0;
        for(i = 0; i< count;i++){
            struct  LOG cmd = hdbRecord.record.logs.log[i];
            cout << "ioa:" <<cmd.log_type << " tm:" << cmd.tm << " value:" << cmd.value << endl;
        }
    }


    devHisManage = new DevHisManage(".");
//    for(int i = 0; i < 1000; i++){
    struct  HDB_RECORD addRecord;
    addRecord.file_type = FT_LOG;
    addRecord.record.logs.num = 1;
    addRecord.record.logs.log[0].log_type = 7;
    addRecord.record.logs.log[0].value = 1;
    addRecord.record.logs.log[0].tm = (char *)"160813_180000_011";
    addRecord.record.logs.log[0].text = (char *)"日志5";
    result = devHisManage->update_record(&addRecord,5);
    cout<< "更新结果:"  << result << endl;
//    }

    devHisManage = new DevHisManage(".");
    devHisManage->read_record(FT_LOG,&hdbRecord);
    i = 0;
    count = hdbRecord.record.logs.num+1;
    for(i = 0; i< count;i++){
        struct  LOG event = hdbRecord.record.logs.log[i];
        cout << "logType:" <<event.log_type << " tm:" << event.tm << " value:" << event.value << endl;
    }
    devHisManage = new DevHisManage(".");
    cout << "读取条数"<<devHisManage->read_record_num(FT_LOG) << endl;
}


int main() {

	testSoe(); //insert之前打印所有记录，insert之后打印所有记录,读取记录条数
	//testCMD(); //insert之后打印所有记录,读取记录条数
    //testLog();//更新一条记录，而后打印所有记录,读取记录条数
    return 0;
}

