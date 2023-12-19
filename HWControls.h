// New LEDs

#define ALL_NOTES_OFF_LED 0
#define NOTE_OFF_LED 1
#define NOTE_ON_LED 2
#define POLY_PRESSURE_LED 3
#define CONTROL_LED 4
#define CHANNEL_PRESSURE_LED 5
#define PITCHBEND_LED 6
#define SYS_EXC_LED 7

#define SONG_POS_LED 8
#define SONG_SEL_LED 9
#define TUNE_REQ_LED 10
#define SUSTAIN_LED 11
#define TIMING_CLOCK_LED 12
#define START_LED 13
#define STOP_LED 14
#define CONTINUE_LED 15

#define CHANNEL_16_LED 16
#define CHANNEL_15_LED 17
#define CHANNEL_14_LED 18
#define CHANNEL_13_LED 19
#define CHANNEL_12_LED 20
#define CHANNEL_11_LED 21
#define CHANNEL_10_LED 22
#define CHANNEL_9_LED 23

#define CHANNEL_8_LED 24
#define CHANNEL_7_LED 25
#define CHANNEL_6_LED 26
#define CHANNEL_5_LED 27
#define CHANNEL_4_LED 28
#define CHANNEL_3_LED 29
#define CHANNEL_2_LED 30
#define CHANNEL_1_LED 31


//Teensy 4.1 Pins

#define ACT_SENSE_LED 8
#define SYSTEM_RESET_LED 9
#define MIDI_LED 4
#define PROGRAM_LED 23
#define MODULATION_LED 22
#define ERROR_LED 17

#define BUTTON_1 18
#define BUTTON_2 19
#define BUTTON_3 20

void setupHardware() {

  //Mux address pins

  pinMode (ACT_SENSE_LED, OUTPUT);
  pinMode (SYSTEM_RESET_LED, OUTPUT);
  pinMode (MIDI_LED, OUTPUT);
  pinMode (PROGRAM_LED, OUTPUT);
  pinMode (MODULATION_LED, OUTPUT);

  digitalWrite(ACT_SENSE_LED, LOW);
  digitalWrite(SYSTEM_RESET_LED, LOW);
  digitalWrite(MIDI_LED, LOW);
  digitalWrite(PROGRAM_LED, LOW);
  digitalWrite(MODULATION_LED, LOW);
}

