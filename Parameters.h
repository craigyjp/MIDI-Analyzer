const char* VERSION = "V1.4";

elapsedMillis ledOnMillis;

int chosenMessage = 0;
int valueMessage = 0;
int outChannel = 1;
int paramNumber = -1;
int oldparamNumber = -1;
int note_filter;
int aftertouch_filter;
int bend_filter;
int mod_filter;
int program_filter;
int realtime_filter;
int system_filter;
int controllers_filter;
int active_filter;
int sustain_filter;
int all_notes_off_filter;
boolean sendData = false;
boolean sendMessage = false;
boolean data1Select = false;
boolean data2Select = false;
boolean data3Select = false;
boolean paramChange = false;
boolean hasValue = false;
boolean refresh_screen = false;
uint16_t pitchBendValue = 0;
int16_t signedPitchBendValue = 0;

#define EEPROM_NOTES 0
#define EEPROM_AFTERTOUCH 1
#define EEPROM_PITCHBEND 2
#define EEPROM_MODULATION 3
#define EEPROM_PROGRAM 4
#define EEPROM_REALTIME 5
#define EEPROM_SYSTEM 6
#define EEPROM_CONTROLLERS 7
#define EEPROM_ACTIVE 8
#define EEPROM_SUSTAIN 9
#define EEPROM_NOTES_OFF 10

static unsigned long display_timer = 0;
static unsigned long noteOn_timer = 0;
static unsigned long noteOff_timer = 0;
static unsigned long control_timer = 0;
static unsigned long program_timer = 0;
static unsigned long modulation_timer = 0;
static unsigned long pitchbend_timer = 0;
static unsigned long polyPressure_timer = 0;
static unsigned long allNotes_timer = 0;
static unsigned long channelPressure_timer = 0;
static unsigned long sysex_timer = 0;
static unsigned long sustain_timer = 0;
static unsigned long songpos_timer = 0;
static unsigned long songstart_timer = 0;
static unsigned long songstop_timer = 0;
static unsigned long songcontinue_timer = 0;
static unsigned long songselect_timer = 0;
static unsigned long tuneRequest_timer = 0;
static unsigned long activeSensing_timer = 0;
static unsigned long systemReset_timer = 0;
static unsigned long clock_timer = 0;
static unsigned long error_timer = 0;
static unsigned long channel1_timer = 0;
static unsigned long channel2_timer = 0;
static unsigned long channel3_timer = 0;
static unsigned long channel4_timer = 0;
static unsigned long channel5_timer = 0;
static unsigned long channel6_timer = 0;
static unsigned long channel7_timer = 0;
static unsigned long channel8_timer = 0;
static unsigned long channel9_timer = 0;
static unsigned long channel10_timer = 0;
static unsigned long channel11_timer = 0;
static unsigned long channel12_timer = 0;
static unsigned long channel13_timer = 0;
static unsigned long channel14_timer = 0;
static unsigned long channel15_timer = 0;
static unsigned long channel16_timer = 0;
