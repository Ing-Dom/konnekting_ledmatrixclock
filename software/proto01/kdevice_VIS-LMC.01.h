#define MANUFACTURER_ID 7337
#define DEVICE_ID 60
#define REVISION 0

#define COMOBJ_switch 0
#define COMOBJ_stat_switch 1
#define COMOBJ_function 2
#define COMOBJ_color 3
#define COMOBJ_backcolor 4
#define COMOBJ_scene 5
#define COMOBJ_gen_daynight 6
#define COMOBJ_gen_datetime 7
#define COMOBJ_error_code 8
#define PARAM_param1 0
#define PARAM_param2 1
        
KnxComObject KnxDevice::_comObjectsList[] = {
    /* Index 0 - switch */ KnxComObject(KNX_DPT_1_001, 0x28),
    /* Index 1 - stat_switch */ KnxComObject(KNX_DPT_1_001, 0x34),
    /* Index 2 - function */ KnxComObject(KNX_DPT_5_010, 0x38),
    /* Index 3 - color */ KnxComObject(KNX_DPT_232_600, 0x28),
    /* Index 4 - backcolor */ KnxComObject(KNX_DPT_232_600, 0x28),
    /* Index 5 - scene */ KnxComObject(KNX_DPT_18_001, 0x38),
    /* Index 6 - gen_daynight */ KnxComObject(KNX_DPT_1_001, 0x2a),
    /* Index 7 - gen_datetime */ KnxComObject(KNX_DPT_19_001, 0x2a),
    /* Index 8 - error_code */ KnxComObject(KNX_DPT_12_001, 0x34)
};
const byte KnxDevice::_numberOfComObjects = sizeof (_comObjectsList) / sizeof (KnxComObject); // do not change this code
       
byte KonnektingDevice::_paramSizeList[] = {
    /* Index 0 - param1 */ PARAM_UINT8,
    /* Index 1 - param2 */ PARAM_UINT8
};
const int KonnektingDevice::_numberOfParams = sizeof (_paramSizeList); // do not change this code