#coding=gbk
#!/usr/bin/env python
from __future__ import with_statement
from xml.dom.minidom import Document
import xml.etree.ElementTree as ET
from shutil import copy
import os
import zlib


#ԭʼ�������ò�������ͼidС��������֣�
COUNTRY_BASE_ID = 1000

#�����ļ�����
MONSTER_INFO = "moninfo"
NPC_INFO = "npcinfo"
JUMPPOINT_INFO = "jumppoint"

#ƴ�ӳ�ȫ·��
SEARCH_ROOT_DIR = "./bin/datas/"
MONSTER_SEARCH_DIR = SEARCH_ROOT_DIR + MONSTER_INFO + "/"
NPC_SEARCH_DIR = SEARCH_ROOT_DIR + NPC_INFO + "/"
JUMPPOINT_SEARCH_DIR = SEARCH_ROOT_DIR + JUMPPOINT_INFO + "/"

#����Ƿ���Ҫ������������ĵ�ͼ���
def checkSkillMap(mapid):
    if mapid == 1000000:
        print( "������ͼ���: %d"%(mapid) )
        return True
    return False

#�������ֲ���Ϣ
def handleMonsterInfo(countryStartId):
    #print("���ڸ��ƹ������ݣ������ĵȴ�......")
    for path in os.walk(MONSTER_SEARCH_DIR):
        for file in path[2]:
            startPos = len(MONSTER_INFO)
            endPos = file.index(".xml")
            mapid = int( file[startPos:endPos] )
            if checkSkillMap(mapid):
                continue;
            if mapid >= COUNTRY_BASE_ID:
                continue
            srcfile = MONSTER_SEARCH_DIR + file
            destfile = "%s%s%d.xml"%(MONSTER_SEARCH_DIR,MONSTER_INFO,mapid+countryStartId)
            copy(srcfile, destfile)


#����npc�ֲ���Ϣ
def handleNpcInfo(countryStartId):
    #print("���ڸ���npc���ݣ������ĵȴ�......")
    for path in os.walk(NPC_SEARCH_DIR):
        for file in path[2]:
            startPos = len(NPC_INFO)
            endPos = file.index(".xml")
            mapid = int( file[startPos:endPos] )
            if checkSkillMap(mapid):
                continue;
            if mapid >= COUNTRY_BASE_ID:
                continue
            srcfile = NPC_SEARCH_DIR + file
            destfile = "%s%s%d.xml"%(NPC_SEARCH_DIR,NPC_INFO,mapid+countryStartId)
            copy(srcfile, destfile)


#�����͵��ļ����ݵ��޸�
def editJumpPointInfo(xmlfile, countryStartId):
    #print("�����޸���ת�����ݣ������ĵȴ�......")
    #print(xmlfile)
    #print(countryStartId)
    tree = ET.parse(xmlfile)
    root = tree.getroot()
    for child in root.iter('point'):
        mapid = int(child.get('TargetMapId'))
        if mapid >= COUNTRY_BASE_ID:
            continue
        mapid = mapid + countryStartId
        child.set('TargetMapId', "%d"%(mapid))
        #print( child.get('TargetMapId') )
        #print( child.attrib )
    tree.write(xmlfile)

#�����͵�ֲ���Ϣ(�����Ҫ�޸�xml����)
def handleJumpPointInfo(countryStartId):
    #print("���ڸ�����ת�����ݣ������ĵȴ�......")
    for path in os.walk(JUMPPOINT_SEARCH_DIR):
        for file in path[2]:
            #print(file)
            startPos = len(JUMPPOINT_INFO)
            endPos = file.index(".xml")
            mapid = int( file[startPos:endPos] )
            if checkSkillMap(mapid):
                continue;
            if mapid >= COUNTRY_BASE_ID:
                continue
            srcfile = JUMPPOINT_SEARCH_DIR + file
            destfile = "%s%s%d.xml"%(JUMPPOINT_SEARCH_DIR,JUMPPOINT_INFO,mapid+countryStartId)
            #print(srcfile)
            #print(destfile)
            copy(srcfile, destfile)
            editJumpPointInfo(destfile, countryStartId)

def main():
    #print("���ڸ��ƹ������ݣ������ĵȴ�......")
    
    #���ƹ���2
    handleMonsterInfo(1000)
    handleNpcInfo(1000)
    handleJumpPointInfo(1000)

    #���ƹ���3
    #handleMonsterInfo(2000)
    #handleNpcInfo(2000)
    #handleJumpPointInfo(2000)

    print("�������ݸ�����ϣ�����")

if __name__ == "__main__":
    main()

