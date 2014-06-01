#coding=gbk
#!/usr/bin/python
import os

#生成ui项目文件
def main():
    #读取模板文件
    file_object = open("templete.ui","r")
    all_the_text = file_object.read()
    file_object.close();
    #print all_the_text
    file_list_string = ""
    #遍历json文件名
    for path in os.walk("./Json/"):
        for file in path[2]:
            file_list_string = file_list_string + "\t<string>" + file + "</string>\r\n"
    #替换到模板中
    out_text = all_the_text.replace('{FILE_LIST}', file_list_string)
    #print out_text
    #生成ui项目文件
    file_object = open("gui.xml.ui","w")
    file_object.write(out_text)
    file_object.close()

if __name__ == '__main__':
    main()
