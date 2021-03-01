#ifndef BT_UISENSOR_H
#define BT_UISENSOR_H

typedef struct Read_par{
	int fd[10]; //打开的uart
	int len[10];//超声：1-5sensor返回的距离。
}read_par;

struct Read_par get_dis_val(read_par par);

#endif

