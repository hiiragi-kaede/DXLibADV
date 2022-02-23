from pprint import pprint
import os

line_break_size=41
def line_break(text):
    ret=text
    size,cnt=len(ret),0
    while(size>(cnt+1)*line_break_size+cnt):
        insert_point=(cnt+1)*line_break_size+cnt
        ret=ret[:insert_point]+"\\n"+ret[insert_point:]
        cnt+=1
    return ret
    
#input_file=input("加工前のファイル名を入力してください:")
files=os.listdir("./")
files=[i for i in files if ".txt" in i]
for input_file in files:
    with open(input_file) as f:
        #aiノベリストで生成した文書を読み込みやすいjsonファイルに加工する。
        data=f.readlines()
        
        output_file=input_file.split(".")[0]+".json"
        with open(output_file,"w") as f_out:
            print('{\n\t"contents":[',file=f_out)
            for ele in data[:-1]:
                ele=ele.replace("\n","")
                
                #1行が長すぎる場合改行を挿入する。
                ele=line_break(ele)
                print('\t\t{"content":"'+ele+'"},',file=f_out)
            
            text=line_break(data[-1])
            print('\t\t{"content":"'+text+'"}',file=f_out)
            print('\t]\n}',file=f_out)