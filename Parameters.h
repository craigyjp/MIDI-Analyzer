const char* VERSION = "V1.3";

elapsedMillis ledOnMillis;

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

boolean paramChange = false;
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

boolean bank0 = 0;
boolean bank1 = 0;
boolean bank2 = 0;
boolean bank3 = 0;
int i = 0;
int currentbank = 0;

//OLED display data table
//PRESET 1st line
const char str01[8][12] PROGMEM = {//str1 is effect name of 1st line
 "Chorus", "Flange", "Tremolo", "Pitch", "Pitch", "No Effect", "Reverb", "Reverb"
};
const char* const name01[] PROGMEM = {
 str01[0], str01[1], str01[2], str01[3], str01[4], str01[5], str01[6], str01[7],
};

//PRESET 2nd line
const char str02[8][12] PROGMEM = {//str1 is effect name of 1st line
 "Reverb", "Reverb", "Reverb", "Shift", "Echo", " ", "1", "2"
};
const char* const name02[] PROGMEM = {
 str02[0], str02[1], str02[2], str02[3], str02[4], str02[5], str02[6], str02[7],
};

//PRESET param1
const char str03[8][12] PROGMEM = {//str1 is effect name of 1st line
 "rev mix", "rev mix", "rev mix", "pitch", "pitch", "-", "rev time", "rev time"
};
const char* const name03[] PROGMEM = {
 str03[0], str03[1], str03[2], str03[3], str03[4], str03[5], str03[6], str03[7],
};

//PRESET param2
const char str04[8][12] PROGMEM = {//str1 is effect name of 1st line
 "cho rate", "flng rate", "trem rate", "-", "echo delay", "-", "HF filter", "HF filter"
};
const char* const name04[] PROGMEM = {
 str04[0], str04[1], str04[2], str04[3], str04[4], str04[5], str04[6], str04[7],
};

//PRESET param3
const char str05[8][12] PROGMEM = {//str1 is effect name of 1st line
 "cho mix", "flng mix", "trem mix", "-", "echo mix", "-", "LF filter", "LF filter"
};
const char* const name05[] PROGMEM = {
 str05[0], str05[1], str05[2], str05[3], str05[4], str05[5], str05[6], str05[7],
};

//ROM1 1st line
const char str11[8][12] PROGMEM = {//str1 is effect name of 1st line
"Qubit","SuperNova","Modulated","Echo","Shimmer","Sample &","Modulated","Glitch Bit"
};
const char* const name11[] PROGMEM = {
 str11[0], str11[1], str11[2], str11[3], str11[4], str11[5], str11[6], str11[7],
};

//ROM1 2nd line
const char str12[8][12] PROGMEM = {//str1 is effect name of 1st line
"Delay","Delay","Delay","Reverb","Reverb","Hold","Reverb","Delay"
};
const char* const name12[] PROGMEM = {
 str12[0], str12[1], str12[2], str12[3], str12[4], str12[5], str12[6], str12[7],
};

//ROM1 param1
const char str13[8][12] PROGMEM = {//str1 is effect name of 1st line
"time","time","time","rev level","dwell","time","dwell","delay 1"

};
const char* const name13[] PROGMEM = {
 str13[0], str13[1], str13[2], str13[3], str13[4], str13[5], str13[6], str13[7],
};

//ROM1 param2
const char str14[8][12] PROGMEM = {//str1 is effect name of 1st line
"modulation","filter","feedback","delay time","pitch","feedback","depth","delay 2"

};
const char* const name14[] PROGMEM = {
 str14[0], str14[1], str14[2], str14[3], str14[4], str14[5], str14[6], str14[7],
};

//ROM1 param3
const char str15[8][12] PROGMEM = {//str1 is effect name of 1st line
"feedback","feedback","modulation","echo level","blend","modulation","rate","feedback"

};
const char* const name15[] PROGMEM = {
 str15[0], str15[1], str15[2], str15[3], str15[4], str15[5], str15[6], str15[7],
};

//ROM2 1st line
const char str21[8][12] PROGMEM = {//str1 is effect name of 1st line
"Daydream","Starfield","Dual Pitch","Triple","Reverse","Wah","Vibrato","Phaser"
};
const char* const name21[] PROGMEM = {
 str21[0], str21[1], str21[2], str21[3], str21[4], str21[5], str21[6], str21[7],
};

//ROM2 2nd line
const char str22[8][12] PROGMEM = {//str1 is effect name of 1st line
"Delay","Delay","Shift","Delay","Delay","Reverb","Reverb","Reverb"
};
const char* const name22[] PROGMEM = {
 str22[0], str22[1], str22[2], str22[3], str22[4], str22[5], str22[6], str22[7],
};

//ROM2 param1
const char str23[8][12] PROGMEM = {//str1 is effect name of 1st line
"time","time","pitch 1","delay 1","sample","reverb","reverb","reverb"
};
const char* const name23[] PROGMEM = {
 str23[0], str23[1], str23[2], str23[3], str23[4], str23[5], str23[6], str23[7],
};

//ROM2 param2
const char str24[8][12] PROGMEM = {//str1 is effect name of 1st line
"feedback","feedback","1-mix-2","delay 2","feedback","rate","rate","rate"
};
const char* const name24[] PROGMEM = {
 str24[0], str24[1], str24[2], str24[3], str24[4], str24[5], str24[6], str24[7],
};

//ROM2 param3
const char str25[8][12] PROGMEM = {//str1 is effect name of 1st line
"filter","phaser","pitch 2","delay 3","delay","wah","vibrato","phaser"
};
const char* const name25[] PROGMEM = {
 str25[0], str25[1], str25[2], str25[3], str25[4], str25[5], str25[6], str25[7],
};

//ROM3 1st line
const char str31[8][12] PROGMEM = {//str1 is effect name of 1st line
"Phaser","Flanger","VP330","Cathedral","Rotor","Ensemble","Leslie","Wah Wah"
};
const char* const name31[] PROGMEM = {
 str31[0], str31[1], str31[2], str31[3], str31[4], str31[5], str31[6], str31[7],
};

//ROM3 2nd line
const char str32[8][12] PROGMEM = {//str1 is effect name of 1st line
"Bass","Bass","Ensemble","","Effect","Effect","Effect",""
};
const char* const name32[] PROGMEM = {
 str32[0], str32[1], str32[2], str32[3], str32[4], str32[5], str32[6], str32[7],
};

//ROM3 param1
const char str33[8][12] PROGMEM = {//str1 is effect name of 1st line
"rate","rate","rev level","rev level","rev level","rev level","rev level","rev level"
};
const char* const name33[] PROGMEM = {
 str33[0], str33[1], str33[2], str33[3], str33[4], str33[5], str33[6], str33[7],
};

//ROM3 param2
const char str34[8][12] PROGMEM = {//str1 is effect name of 1st line
"depth","depth","cho/ens mix","feedback","depth","ens mix","filter freq","filter Q"
};
const char* const name34[] PROGMEM = {
 str34[0], str34[1], str34[2], str34[3], str34[4], str34[5], str34[6], str34[7],
};

//ROM3 param3
const char str35[8][12] PROGMEM = {//str1 is effect name of 1st line
"feedback","feedback","ensemble","speed","speed","treble","speed","sensitivity"
};
const char* const name35[] PROGMEM = {
 str35[0], str35[1], str35[2], str35[3], str35[4], str35[5], str35[6], str35[7],
};