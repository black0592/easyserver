#coding=gbk
#!/usr/bin/env python
from __future__ import with_statement
from xml.dom.minidom import Document
import xml.etree.ElementTree as ET
from shutil import copy
import os
import zlib


#原始国家配置参数（地图id小于这个数字）
COUNTRY_BASE_ID = 1000

#功能文件夹名
MONSTER_INFO = "moninfo"
NPC_INFO = "npcinfo"
JUMPPOINT_INFO = "jumppoint"

#拼接出全路径
SEARCH_ROOT_DIR = "./bin/datas/"
MONSTER_SEARCH_DIR = SEARCH_ROOT_DIR + MONSTER_INFO + "/"
NPC_SEARCH_DIR = SEARCH_ROOT_DIR + NPC_INFO + "/"
JUMPPOINT_SEARCH_DIR = SEARCH_ROOT_DIR + JUMPPOINT_INFO + "/"

#检查是否需要跳过，不处理的地图编号
def checkSkillMap(mapid):
    if mapid == 1000000:
        print( "跳过地图编号: %d"%(mapid) )
        return True
    return False

#处理怪物分布信息
def handleMonsterInfo(countryStartId):
    #print("正在复制怪物数据，请耐心等待......")
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


#处理npc分布信息
def handleNpcInfo(countryStartId):
    #print("正在复制npc数据，请耐心等待......")
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


#处理传送点文件内容的修改
def editJumpPointInfo(xmlfile, countryStartId):
    #print("正在修改跳转点数据，请耐心等待......")
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

#处理传送点分布信息(这个需要修改xml内容)
def handleJumpPointInfo(countryStartId):
    #print("正在复制跳转点数据，请耐心等待......")
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
    #print("正在复制国家数据，请耐心等待......")
    
    #复制国家2
    handleMonsterInfo(1000)
    handleNpcInfo(1000)
    handleJumpPointInfo(1000)

    #复制国家3
    #handleMonsterInfo(2000)
    #handleNpcInfo(2000)
    #handleJumpPointInfo(2000)

    print("国家数据复制完毕！！！")

if __name__ == "__main__":
    main()

