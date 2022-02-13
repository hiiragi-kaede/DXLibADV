from pprint import pprint

input_file=input("加工前のファイル名を入力してください:")
with open(input_file) as f:
    #aiノベリストで生成した文書を読み込みやすいjsonファイルに加工する。
    #単純に1行をそのままテキストボックスに入れられる前提で加工するので、はみ出しに注意。
    # (後で文字数をカウントして改行文字を挿入する機能を追加するかも)
    data=f.readlines()
    
    output_file=input_file.split(".")[0]+".json"
    with open(output_file,"w") as f_out:
        print('{\n\t"contents":[',file=f_out)
        for ele in data[:-1]:
            ele=ele.replace("\n","")
            print('\t\t{"content":"'+ele+'"},',file=f_out)
        print('\t\t{"content":"'+data[-1]+'"}',file=f_out)
        print('\t]\n}',file=f_out)