#ifndef _U_CAM_H_
#define _U_CAM_H_

struct Ucamera_Product_Cfg {
    int Pid;
    int Vid;
    int version;
    char manufacturer[64];
    char product[64];
    char serial[64];
    char video_name[64];
    char audio_name[64];
};


#endif // !_U_CAM_H_
