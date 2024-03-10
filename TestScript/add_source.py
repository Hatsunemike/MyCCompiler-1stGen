temp_file = open("./Template.asm",encoding = "utf-8",mode = "r")
src_file  = open("./source.txt",  encoding = "utf-8",mode = "r")
tgt_file  = open("./target.asm",  encoding = "utf-8",mode = "w")

while (True):
    my_str = temp_file.readline()
    if(my_str == "") :break
    if(my_str == " [Here put your code.]\n") :
        tmp_str = src_file.readline()
        while tmp_str != "":
            tgt_file.write(tmp_str)
            tmp_str = src_file.readline()
    else :
        tgt_file.write(my_str)


temp_file.close()
src_file.close()