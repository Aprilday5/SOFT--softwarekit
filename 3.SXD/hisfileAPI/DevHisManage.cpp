//
// Created by dwolf on 2020/2/8.
//

#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#include "DevHisManage.h"
#include "HisFileCommon.h"


int DevHisManage::read_record_num(enum FILE_TYPE fileType) {
    string fileName = this->filePath+this->genrateFileName(fileType);
    XMLDocument doc;
    if ( doc.LoadFile(fileName.c_str()) )
    {
        doc.PrintError();
        return 0;
    }

    // 根元素
    XMLElement* root = doc.RootElement();
    XMLElement* headerEle = root->FirstChildElement( "DataRec");
    int count = 0;
    headerEle->QueryIntAttribute("num", &count);
    return count;
}

int DevHisManage::read_record(enum FILE_TYPE file_type, struct HDB_RECORD *record) {
    string fileName = this->filePath+this->genrateFileName(file_type);
    return this->readFile(fileName,*&record);
}

int DevHisManage::insert_record(struct HDB_RECORD *record) {
    string fileName = this->filePath+this->genrateFileName(record->file_type);
    return this->saveFile(fileName,record,0,0);
}

int DevHisManage::update_record(struct HDB_RECORD *record, int no) {
    string fileName = this->filePath+this->genrateFileName(record->file_type);
    return this->saveFile(fileName,record,no,1);
}

int DevHisManage::saveFile(string fileName,struct HDB_RECORD *record,int no,int type) {
    XMLDocument doc;
    if(XML_ERROR_FILE_NOT_FOUND != doc.LoadFile(fileName.c_str()))
    {
        cout<<"file is exits！"<<endl;
        // 根元素
        XMLElement* root = doc.RootElement();
        XMLElement* dataRec = root->FirstChildElement("DataRec");
        int count = 0;
        dataRec->QueryIntAttribute("num", &count);

        if(type == 0){//插入
            XMLElement* subEle = doc.NewElement("DI");
            if(record->file_type == FT_SOE){
                this->genrateSOEXmlEle(subEle, record->record.events.event[0]);
            }else if(record->file_type == FT_LOG){
                this->genrateLogXmlEle(subEle, record->record.logs.log[0]);
            }else if(record->file_type == FT_CO){
                this->genrateCOXmlEle(subEle, record->record.cmds.cmd[0]);
            }
            dataRec->InsertFirstChild(subEle);
            if(count >= 1023){
                int i = 1;
                XMLElement* currentele =dataRec->FirstChildElement();
                XMLElement* tmpEle;
                for (XMLElement* nextEle = currentele->NextSiblingElement(); nextEle; )
                {
                    if(i>1023){
                        tmpEle = nextEle;
                        nextEle = nextEle->NextSiblingElement();
                        dataRec->DeleteChild(tmpEle);

                    }else{
                        nextEle = nextEle->NextSiblingElement();
                    }
                    i++;
                }
                count = 1023;
            }
            count++;
            dataRec->SetAttribute("num",count);
        }else{ //更新
            int i = 1;
            bool hasEle = false;
            for (XMLElement* currentele =dataRec->FirstChildElement();currentele; currentele = currentele->NextSiblingElement() )
            {
                if(i == no){
                    if(record->file_type == FT_SOE){
                        this->genrateSOEXmlEle(currentele, record->record.events.event[0]);
                    }else if(record->file_type == FT_LOG){
                        this->genrateLogXmlEle(currentele, record->record.logs.log[0]);
                    }else if(record->file_type == FT_CO){
                        this->genrateCOXmlEle(currentele, record->record.cmds.cmd[0]);
                    }
                    hasEle = true;
                    break;
                }
                i++;
            }
            if(!hasEle){
                return -1;
            }
        }
        return doc.SaveFile(fileName.c_str());
    }else{
        string tmpPath ;
        if(record->file_type == FT_SOE){
            tmpPath = "/SOE";
        }else if(record->file_type == FT_LOG){
            tmpPath = "/ULOG";
        }else if(record->file_type == FT_CO){
            tmpPath = "/CO";
        }
        if(access((this->filePath+tmpPath).c_str(),0)==-1)  //access函数是查看文件是不是存在
        {
            if (mkdir((this->filePath+tmpPath).c_str(),0777)) //如果不存在就用mkdir函数来创建
            {
                printf("creat file bag failed!!!\n");
                return -1;
            }
        }

        XMLDocument *newDoc = new XMLDocument();
        XMLDeclaration *declaration = newDoc->NewDeclaration();
        newDoc->InsertFirstChild(declaration);
        //3.新建root根节点
        XMLElement *root = newDoc->NewElement("DataFile");
        newDoc->InsertEndChild(root);
        //4.给root添加节点
        XMLElement* headNode = newDoc->NewElement("Header");
        //4.1设置属性
        string fileType = "";
        if(record->file_type == FT_SOE){
            fileType="SOE";
        }else if(record->file_type == FT_LOG){
            fileType="Ulog";
        }else if(record->file_type == FT_CO){
            fileType="CO";
        }
        headNode->SetAttribute("fileType",fileType.c_str());
        headNode->SetAttribute("fileVer",1.00);
        headNode->SetAttribute("devName","测试");
        root->InsertFirstChild(headNode);

        XMLElement* dataNode = newDoc->NewElement("DataRec");

        if(record->file_type == FT_SOE){
            dataNode->SetAttribute("num",record->record.events.num);
            this->genrateSOEXml(newDoc,dataNode,record->record.events.event,record->record.events.num);
        }else if(record->file_type == FT_LOG){
            dataNode->SetAttribute("num",record->record.logs.num);
            this->genrateLogXml(newDoc,dataNode,record->record.logs.log,record->record.logs.num);
        }else if(record->file_type == FT_CO){
            dataNode->SetAttribute("num",record->record.cmds.num);
            this->genrateCOXml(newDoc,dataNode,record->record.cmds.cmd,record->record.cmds.num);
        }
        //4.3设置结束标签
        root->InsertEndChild(dataNode);
        return newDoc->SaveFile(fileName.c_str());
    }

    return 0;
}

void DevHisManage::genrateCOXmlEle(XMLElement* diNode, struct CMD event){
    string a;
    char temp[20];
    sprintf(temp,"%lu",event.info_addr);
    diNode->SetAttribute("ioa",temp);
    diNode->SetAttribute("tm",event.tm);

    diNode->SetAttribute("cmd",event.cmd);
    diNode->SetAttribute("val",event.value);
}

void DevHisManage::genrateLogXmlEle(XMLElement* diNode, struct LOG event){
    diNode->SetAttribute("logType",event.log_type);
    diNode->SetAttribute("tm",event.tm);
    diNode->SetAttribute("txt",event.text);
    diNode->SetAttribute("val",event.value);
}

void DevHisManage::genrateSOEXmlEle(XMLElement* diNode, struct EVENT event){
    diNode->SetAttribute("ioa",event.info_addr);
    diNode->SetAttribute("tm",event.tm);
    diNode->SetAttribute("val",event.value);
}



int DevHisManage::genrateCOXml(XMLDocument* doc,XMLElement* root, struct CMD events[], int count){
    XMLElement* tmpNode;
    for( int i = 0; i< count; i++){
        XMLElement* diNode = doc->NewElement("DI");
        struct CMD event = events[i];
        string a;
        char temp[20];

        sprintf(temp,"%lu",event.info_addr);
        diNode->SetAttribute("ioa",temp);
        diNode->SetAttribute("tm",event.tm);

        diNode->SetAttribute("cmd",event.cmd);
        diNode->SetAttribute("val",event.value);
        if(i == 0){
            root->InsertFirstChild(diNode);
        }else if(i == count -1){
            root->InsertEndChild(diNode);
        }else{
            root->InsertAfterChild(tmpNode,diNode);
        }
        tmpNode = diNode;
    }
    return 1;
}

int DevHisManage::genrateLogXml(XMLDocument* doc,XMLElement* root, struct LOG events[], int count){
    XMLElement* tmpNode;
    for( int i = 0; i< count; i++){
        XMLElement* diNode = doc->NewElement("DI");
        struct LOG event = events[i];
        diNode->SetAttribute("logType",event.log_type);
        diNode->SetAttribute("tm",event.tm);
        diNode->SetAttribute("txt",event.text);
        diNode->SetAttribute("val",event.value);
        if(i == 0){
            root->InsertFirstChild(diNode);
        }else if(i == count -1){
            root->InsertEndChild(diNode);
        }else{
            root->InsertAfterChild(tmpNode,diNode);
        }
        tmpNode = diNode;
    }
    return 1;
}


int DevHisManage::genrateSOEXml(XMLDocument* doc,XMLElement* root, struct EVENT events[], int count){
    XMLElement* tmpNode;
    for( int i = 0; i< count; i++){
        XMLElement* diNode = doc->NewElement("DI");
        struct EVENT event = events[i];
        diNode->SetAttribute("ioa",event.info_addr);
        diNode->SetAttribute("tm",event.tm);
        diNode->SetAttribute("val",event.value);
        if(i == 0){
            root->InsertFirstChild(diNode);
        }else if(i == count -1){
            root->InsertEndChild(diNode);
        }else{
            root->InsertAfterChild(tmpNode,diNode);
        }
        tmpNode = diNode;
    }
    return 1;

}


int DevHisManage::readFile(string fileName, struct HDB_RECORD* record) {
    XMLDocument doc;
    if ( doc.LoadFile(fileName.c_str()) )
    {
        doc.PrintError();
        return -1;
    }

    // 根元素
    XMLElement* root = doc.RootElement();

    cout << "name:" << root->Name()  << endl;

    // 遍历<surface>元素
    XMLElement* headerEle = root->FirstChildElement( "Header");
    const char *fileType = new char[100];
    headerEle->QueryStringAttribute("fileType", &fileType);
    cout << "fielType:" << fileType << endl;
    if(0 == strcmp(fileType,"SOE")){
        return this->readSoeFile(root,record);
    }else if(0 == strcmp(fileType,"CO")){
        return this->readCoFile(root,record);
    }else if(0 == strcmp(fileType,"Ulog")){
        return this->readCoFile(root,record);
    }

    return -1;
}

int DevHisManage::readSoeFile(XMLElement *ele, struct HDB_RECORD *record) {
    record->file_type = FT_SOE;
    struct  EVENTS* events = &record->record.events;
    XMLElement* dataRec = ele->FirstChildElement("DataRec");
    events->num = 0;
    if(dataRec == NULL){
        events->num = 0;
        return 1;
    }
    string eleName = "DI";

    int i =0;
    struct EVENT* event = &events->event[0];
    for(XMLElement *ptrElement = dataRec->FirstChildElement(eleName.c_str());ptrElement; ptrElement=ptrElement->NextSiblingElement(eleName.c_str())){
        {
            int64_t ioa ;
            ptrElement->QueryInt64Attribute("ioa", &ioa);
            event->info_addr = ioa;

        }
        {
            const char *tm = new char[100];
            ptrElement->QueryStringAttribute("tm", &tm);
            event->tm = const_cast<char *>(tm);
        }
        {
            unsigned int value ;
            ptrElement->QueryUnsignedAttribute("val", &value);
            event->value = value;
        }
        events->num = i;
        event++;
        i++;
    }
    return 1;
}

int DevHisManage::readCoFile(XMLElement *ele, struct HDB_RECORD *record) {
    record->file_type = FT_CO;
    struct  CMDS* events = &record->record.cmds;
    XMLElement* dataRec = ele->FirstChildElement("DataRec");
    events->num = 0;
    if(dataRec == NULL){
        events->num = 0;
        return 1;
    }
    string eleName = "DI";

    int i =0;
    struct CMD* event = &events->cmd[0];
    for(XMLElement *ptrElement = dataRec->FirstChildElement(eleName.c_str());ptrElement; ptrElement=ptrElement->NextSiblingElement(eleName.c_str())){
        {
            int64_t ioa ;
            ptrElement->QueryInt64Attribute("ioa", &ioa);
            event->info_addr = ioa;

        }
        {
            const char *tm = new char[100];
            ptrElement->QueryStringAttribute("tm", &tm);
            event->tm = const_cast<char *>(tm);
        }
        {
            const char *tm = new char[100];
            ptrElement->QueryStringAttribute("cmd", &tm);
            event->cmd = const_cast<char *>(tm);
        }
        {
            unsigned int value ;
            ptrElement->QueryUnsignedAttribute("val", &value);
            event->value = value;
        }
        events->num = i;
        event++;
        i++;
    }
    return 1;
}

int DevHisManage::readLogFile(XMLElement *ele, struct HDB_RECORD *record) {
    record->file_type = FT_LOG;
    struct  LOGS* events = &record->record.logs;
    XMLElement* dataRec = ele->FirstChildElement("DataRec");
    events->num = 0;
    if(dataRec == NULL){
        events->num = 0;
        return 1;
    }
    string eleName = "DI";

    int i =0;
    struct LOG* event = &events->log[0];
    for(XMLElement *ptrElement = dataRec->FirstChildElement(eleName.c_str());ptrElement; ptrElement=ptrElement->NextSiblingElement(eleName.c_str())){
        {
            int64_t logType ;
            ptrElement->QueryInt64Attribute("logType", &logType);
            event->log_type = logType;

        }
        {
            const char *tm = new char[100];
            ptrElement->QueryStringAttribute("tm", &tm);
            event->tm = const_cast<char *>(tm);
        }
        {
            const char *text = new char[1000];
            ptrElement->QueryStringAttribute("text", &text);
            event->text = const_cast<char *>(text);
        }
        {
            unsigned int value ;
            ptrElement->QueryUnsignedAttribute("val", &value);
            event->value = value;
        }
        events->num = i;
        event++;
        i++;
    }
    return 1;
}

DevHisManage::DevHisManage() {

}

DevHisManage::DevHisManage(string filePath) {
    this->filePath = filePath;

}

string DevHisManage::genrateFileName(enum FILE_TYPE file_type) {
    if(file_type == FT_SOE){
        return "/SOE/soe.xml";
    }else if(file_type == FT_CO){
        return "/CO/co.xml";
    }else if(file_type == FT_LOG){
        return "/ULOG/ulog.xml";
    }
    return "";
}

