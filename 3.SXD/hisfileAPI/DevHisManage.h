//
// Created by dwolf on 2020/2/8.
//

#ifndef HWG_DEVHISMANAGE_H
#define HWG_DEVHISMANAGE_H

#include "HisFileCommon.h"
#include <string>
#include <iostream>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

class DevHisManage {
public:
    /**
     * 读取文件中记录个数
     * @param fileType 文件类型
     * @return -1   错误  >=0 文件中记录个数
     */
    int read_record_num(enum FILE_TYPE fileType);

    /**
     * 读取文件中所有记录
     * @param file_type 文件类型（见上述宏定义）
     * @param record 存放读取记录的结果
     * @return -1:失败 ;0:成功
     */
    int read_record(enum FILE_TYPE file_type,struct HDB_RECORD *record);

    /**
     * 向文件中插入一条记录;当文件不存在时，需要创建文件
     * @param record  要插入的记录的结构体
     * @return -1:失败; 0:成功
     */
    int insert_record(struct HDB_RECORD *record);

    /**
     * 更新文件中某一条记录的内容（当文件不存在时，需要创建文件）
     * @param record 要更新的记录的结构体
     * @param no  要更新的记录的序号
     * @return  -1:失败 1:成功
     */
    int update_record(struct HDB_RECORD *record,int no);


    DevHisManage();
    DevHisManage(string filePath);

private:

    int saveFile(string fileName, struct HDB_RECORD *record,int no,int type);
    int readFile(string fileName,struct HDB_RECORD* record);
    int readSoeFile(XMLElement* ele, struct HDB_RECORD *record);
    int readCoFile(XMLElement *ele, struct HDB_RECORD *record);
    int readLogFile(XMLElement *ele, struct HDB_RECORD *record);

    int genrateSOEXml(XMLDocument* doc,XMLElement* root, struct EVENT events[], int count);
    int genrateCOXml(XMLDocument* doc,XMLElement* root, struct CMD events[], int count);
    int genrateLogXml(XMLDocument* doc,XMLElement* root, struct LOG events[], int count);
    void genrateCOXmlEle(XMLElement* diNode, struct CMD event);
    void genrateLogXmlEle(XMLElement* diNode, struct LOG event);
    void genrateSOEXmlEle(XMLElement* diNode, struct EVENT event);

    string genrateFileName(enum FILE_TYPE file_type);
    string filePath;



};


#endif //HWG_DEVHISMANAGE_H
