
#include "afxdialogex.h"
#include "stdafx.h"
#include "CTs.h"
#include "CTsDlg.h"
#include "resource.h"	
#include"mediaMode.h"
//--------------
#include "config.h"
#include <inttypes.h>
#include <math.h>
#include <limits.h>
#include <signal.h>

extern "C"
{
#include "libavutil/avstring.h"
#include "libavutil/colorspace.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/imgutils.h"
#include "libavutil/dict.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/avassert.h"
#include "libavutil/time.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
#include "libavcodec/avfft.h"
#include "libswresample/swresample.h"
}

#if CONFIG_AVFILTER
# include "libavfilter/avcodec.h"
# include "libavfilter/avfilter.h"
# include "libavfilter/avfiltergraph.h"
# include "libavfilter/buffersink.h"
# include "libavfilter/buffersrc.h"
#endif

#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#include "cmdutils.h"

#include <assert.h>

typedef struct
{
	int  isTS;
	int  programN;
	struct VideoState *m_is;

}sendArg;



//最多存储的帧信息
#define MAX_FRAME_NUM 10000
//最多存储的Packet信息
#define MAX_PACKET_NUM 10000
//URL长度
#define MAX_URL_LENGTH 500


//静态函数只能在声明它的文件当中可见，不能被其他文件所调用，也就是说该静态函数只
//能在其定义的.cpp或.c中调用，在其它.cpp或.c文件的函数里是不能被调用的。”
//需要对static void toggle_pause(VideoState *is)进行封装

//发送“暂停”命令
//Send Command "Pause"
void ffmfc_play_pause();

//发送“逐帧”命令
//Send Command "Step"
void ffmfc_seek_step();

//发送“全屏”命令
//Send Command "FullScreen"
void ffmfc_play_fullcreen();

//发送“前进/后退”命令
//Send Command "Seek"
void ffmfc_seek(int time);

//发送“宽高比”命令
//Send Command "AspectRatio"
void ffmfc_aspectratio(int num,int den);

//发送“大小”命令
//Send Command "WindowSize"
void ffmfc_size(int percentage);

//发送“窗口画面内容”命令
//Send Command "Audio Display Mode"
void ffmfc_audio_display(int mode);

//发送“退出”命令
//Send Command "Quit"
void ffmfc_quit();

//解码主函数
//Main function
int ffmfc_play(LPVOID lpParam);

//复位
//Reset
int ffmfc_reset_index();

//播放进度
//Seek Bar
void ffmfc_seek_bar(int pos);
enum sendtype{ media = 0, ts };
typedef struct
{
	mediaMode *dlgMedia;
	CCTsDlg   *dlgCCTs;
	enum sendtype   dlgtype;
}SendAgrStruct;