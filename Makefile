#定义变量
TARGET:=main						#总目标
SRC:=$(wildcard ./src/*.c)				#获取所有的.c文件
OBJS:=$(patsubst %.c,%.o,$(SRC))	#从.c获取对应中间文件
CC:=arm-linux-gnueabi-gcc	#编译器的名字 如果你是用在开发板 换成arm-linux-gnueabi-gcc
INCS:=-I ./include/		#指定头文件的搜索路径
LIBS:=-L ./lib/		#指定库文件的搜索路径
LIBNAME:=-l font	#指定库的名字

$(TARGET):$(OBJS)
	$(CC) $^ -o $@ $(INCS) $(LIBS) $(LIBNAME)
	cp $(TARGET) /home/china/tftpboot 
#gcc *.o -o main -I -L -l

%.o:%.c  #makefile中的通配符，匹配的是文件名
	$(CC) -c $< -o $@ $(INCS) $(LIBS) $(LIBNAME)

clean:
	rm $(OBJS) $(TARGET)