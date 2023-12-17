#include <MIDI.h>
#include <RoxMux.h>
#include "Parameters.h"
#include "HWControls.h"

#define SR_TOTAL 4
Rox74HC595<SR_TOTAL> sr;

// pins for 74HC595
#define LED_DATA 5   // pin 14 on 74HC595 (DATA)
#define LED_LATCH 7  // pin 12 on 74HC595 (LATCH)
#define LED_CLK 6    // pin 11 on 74HC595 (CLK)
#define LED_PWM -1   // pin 13 on 74HC595

//MIDI 5 Pin DIN
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

void setup() {
  sr.begin(LED_DATA, LED_LATCH, LED_CLK, LED_PWM);
  setupHardware();
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop() {

  bool activity = false;

  if (MIDI.read()) {
    // get a MIDI IN1 (Serial) message
    byte type = MIDI.getType();
    byte channel = MIDI.getChannel();
    byte data1 = MIDI.getData1();
    //byte data2 = MIDI.getData2();

    switch (type) {
      case midi::NoteOff:  // 0x80
        sr.writePin(NOTE_OFF_LED, HIGH);
        noteOff_timer = millis();
        channelLED(channel);
        break;

      case midi::NoteOn:  // 0x90
        sr.writePin(NOTE_ON_LED, HIGH);
        noteOn_timer = millis();
        channelLED(channel);
        break;

      case midi::AfterTouchPoly:  // 0xA0
        sr.writePin(POLY_PRESSURE_LED, HIGH);
        polyPressure_timer = millis();
        channelLED(channel);
        break;

      case midi::ControlChange:  // 0xB0
        switch (data1) {

          case 123:
            sr.writePin(ALL_NOTES_OFF_LED, HIGH);
            allNotes_timer = millis();
            channelLED(channel);
            break;

          case 1:
            digitalWrite(MODULATION_LED, HIGH);
            modulation_timer = millis();
            channelLED(channel);
            break;

          default:
            sr.writePin(CONTROL_LED, HIGH);
            control_timer = millis();
            channelLED(channel);
            break;
        }
        break;

      case midi::ProgramChange:  // 0xC0
        digitalWrite(PROGRAM_LED, HIGH);
        program_timer = millis();
        channelLED(channel);
        break;

      case midi::AfterTouchChannel:  // 0xD0
        sr.writePin(CHANNEL_PRESSURE_LED, HIGH);
        channelPressure_timer = millis();
        channelLED(channel);
        break;

      case midi::PitchBend:  // 0xE0
        sr.writePin(PITCHBEND_LED, HIGH);
        pitchbend_timer = millis();
        channelLED(channel);
        break;

      case midi::SystemExclusive:  // 0xF0
        sr.writePin(SYS_EXC_LED, HIGH);
        sysex_timer = millis();
        channelLED(channel);
        break;

      // case midi::TimeCodeQuarterFrame:  // 0xF1
      //   Serial.print("TimeCode // 0xF1, index=");
      //   Serial.print(data1 >> 4, DEC);
      //   Serial.print(", digit=");
      //   Serial.println(data1 & 15, DEC);
      //   break;

      case midi::SongPosition:  // 0xF2
        sr.writePin(SONG_POS_LED, HIGH);
        songpos_timer = millis();
        break;

      case midi::SongSelect:  // 0xF3
        sr.writePin(SONG_SEL_LED, HIGH);
        songselect_timer = millis();
        break;

      case midi::TuneRequest:  // 0xF6
        sr.writePin(TUNE_REQ_LED, HIGH);
        tuneRequest_timer = millis();
        break;

      case midi::Clock:  // 0xF8
        sr.writePin(TIMING_CLOCK_LED, HIGH);
        clock_timer = millis();
        break;

      case midi::Start:  // 0xFA
        sr.writePin(START_LED, HIGH);
        songstart_timer = millis();
        break;

      case midi::Continue:  // 0xFB
        sr.writePin(CONTINUE_LED, HIGH);
        songcontinue_timer = millis();
        break;

      case midi::Stop:  // 0xFC
        sr.writePin(STOP_LED, HIGH);
        songstop_timer = millis();
        break;

      case midi::ActiveSensing:  // 0xFE
        digitalWrite(ACT_SENSE_LED, HIGH);
        activeSensing_timer = millis();
        break;

      case midi::SystemReset:  // 0xFF
        digitalWrite(SYSTEM_RESET_LED, HIGH);
        systemReset_timer = millis();
        break;

      default:
        Serial.println("Opps, an unknown MIDI message type!");
    }
    activity = true;
  }

  if (usbMIDI.read()) {
    // get a MIDI IN1 (Serial) message
    byte type = usbMIDI.getType();
    byte channel = usbMIDI.getChannel();
    byte data1 = usbMIDI.getData1();
    //byte data2 = usbMIDI.getData2();


    switch (type) {
      case midi::NoteOff:  // 0x80
        sr.writePin(NOTE_OFF_LED, HIGH);
        noteOff_timer = millis();
        channelLED(channel);
        break;

      case midi::NoteOn:  // 0x90
        sr.writePin(NOTE_ON_LED, HIGH);
        noteOn_timer = millis();
        channelLED(channel);
        break;

      case midi::AfterTouchPoly:  // 0xA0
        sr.writePin(POLY_PRESSURE_LED, HIGH);
        polyPressure_timer = millis();
        channelLED(channel);
        break;

      case midi::ControlChange:  // 0xB0
        switch (data1) {

          case 123:
            sr.writePin(ALL_NOTES_OFF_LED, HIGH);
            allNotes_timer = millis();
            channelLED(channel);
            break;

          case 64:
            sr.writePin(SUSTAIN_LED, HIGH);
            sustain_timer = millis();
            channelLED(channel);
            break;

          case 1:
            digitalWrite(MODULATION_LED, HIGH);
            modulation_timer = millis();
            channelLED(channel);
            break;

          default:
            sr.writePin(CONTROL_LED, HIGH);
            control_timer = millis();
            channelLED(channel);
            break;
        }
        break;

      case midi::ProgramChange:  // 0xC0
        digitalWrite(PROGRAM_LED, HIGH);
        program_timer = millis();
        channelLED(channel);
        break;

      case midi::AfterTouchChannel:  // 0xD0
        sr.writePin(CHANNEL_PRESSURE_LED, HIGH);
        channelPressure_timer = millis();
        channelLED(channel);
        break;

      case midi::PitchBend:  // 0xE0
        sr.writePin(PITCHBEND_LED, HIGH);
        pitchbend_timer = millis();
        channelLED(channel);
        break;

      case midi::SystemExclusive:  // 0xF0
        sr.writePin(SYS_EXC_LED, HIGH);
        sysex_timer = millis();
        channelLED(channel);
        break;

      // case midi::TimeCodeQuarterFrame:  // 0xF1
      //   Serial.print("TimeCode // 0xF1, index=");
      //   Serial.print(data1 >> 4, DEC);
      //   Serial.print(", digit=");
      //   Serial.println(data1 & 15, DEC);
      //   break;

      case midi::SongPosition:  // 0xF2
        sr.writePin(SONG_POS_LED, HIGH);
        songpos_timer = millis();
        break;

      case midi::SongSelect:  // 0xF3
        sr.writePin(SONG_SEL_LED, HIGH);
        songselect_timer = millis();
        break;

      case midi::TuneRequest:  // 0xF6
        sr.writePin(TUNE_REQ_LED, HIGH);
        tuneRequest_timer = millis();
        break;

      case midi::Clock:  // 0xF8
        sr.writePin(TIMING_CLOCK_LED, HIGH);
        clock_timer = millis();
        break;

      case midi::Start:  // 0xFA
        sr.writePin(START_LED, HIGH);
        songstart_timer = millis();
        break;

      case midi::Continue:  // 0xFB
        sr.writePin(CONTINUE_LED, HIGH);
        songcontinue_timer = millis();
        break;

      case midi::Stop:  // 0xFC
        sr.writePin(STOP_LED, HIGH);
        songstop_timer = millis();
        break;

      case midi::ActiveSensing:  // 0xFE
        digitalWrite(ACT_SENSE_LED, HIGH);
        activeSensing_timer = millis();
        break;

      case midi::SystemReset:  // 0xFF
        digitalWrite(SYSTEM_RESET_LED, HIGH);
        systemReset_timer = millis();
        break;

      default:
        Serial.println("Opps, an unknown MIDI message type!");
    }
    activity = true;
  }

  sr.update();
  ledsOff();

  // blink the LED when any activity has happened
  if (activity) {
    digitalWriteFast(MIDI_LED, HIGH);  // LED on
    ledOnMillis = 0;
  }
  if (ledOnMillis > 15) {
    digitalWriteFast(MIDI_LED, LOW);  // LED off
  }
}

void ledsOff() {

  if ((noteOn_timer > 0) && (millis() - noteOn_timer > 60)) {
    sr.writePin(NOTE_ON_LED, LOW);
    noteOn_timer = 0;
  }

  if ((noteOff_timer > 0) && (millis() - noteOff_timer > 60)) {
    sr.writePin(NOTE_OFF_LED, LOW);
    noteOff_timer = 0;
  }

  if ((control_timer > 0) && (millis() - control_timer > 60)) {
    sr.writePin(CONTROL_LED, LOW);
    control_timer = 0;
  }

  if ((polyPressure_timer > 0) && (millis() - polyPressure_timer > 60)) {
    sr.writePin(POLY_PRESSURE_LED, LOW);
    polyPressure_timer = 0;
  }

  if ((pitchbend_timer > 0) && (millis() - pitchbend_timer > 60)) {
    sr.writePin(PITCHBEND_LED, LOW);
    pitchbend_timer = 0;
  }

  if ((allNotes_timer > 0) && (millis() - allNotes_timer > 60)) {
    sr.writePin(ALL_NOTES_OFF_LED, LOW);
    allNotes_timer = 0;
  }

  if ((program_timer > 0) && (millis() - program_timer > 60)) {
    digitalWrite(PROGRAM_LED, LOW);
    program_timer = 0;
  }

  if ((modulation_timer > 0) && (millis() - modulation_timer > 60)) {
    digitalWrite(MODULATION_LED, LOW);
    modulation_timer = 0;
  }

  if ((channelPressure_timer > 0) && (millis() - channelPressure_timer > 60)) {
    sr.writePin(CHANNEL_PRESSURE_LED, LOW);
    channelPressure_timer = 0;
  }

  if ((sysex_timer > 0) && (millis() - sysex_timer > 60)) {
    sr.writePin(SYS_EXC_LED, LOW);
    sysex_timer = 0;
  }

  if ((songpos_timer > 0) && (millis() - songpos_timer > 60)) {
    sr.writePin(SONG_POS_LED, LOW);
    songpos_timer = 0;
  }

  if ((songselect_timer > 0) && (millis() - songselect_timer > 60)) {
    sr.writePin(SONG_SEL_LED, LOW);
    songselect_timer = 0;
  }

  if ((songstart_timer > 0) && (millis() - songstart_timer > 60)) {
    sr.writePin(START_LED, LOW);
    songstart_timer = 0;
  }

  if ((songstop_timer > 0) && (millis() - songstop_timer > 60)) {
    sr.writePin(STOP_LED, LOW);
    songstop_timer = 0;
  }

  if ((songcontinue_timer > 0) && (millis() - songcontinue_timer > 60)) {
    sr.writePin(CONTINUE_LED, LOW);
    songcontinue_timer = 0;
  }

  if ((activeSensing_timer > 0) && (millis() - activeSensing_timer > 30)) {
    digitalWrite(ACT_SENSE_LED, LOW);
    activeSensing_timer = 0;
  }

  if ((systemReset_timer > 0) && (millis() - systemReset_timer > 60)) {
    digitalWrite(SYSTEM_RESET_LED, LOW);
    systemReset_timer = 0;
  }

  if ((tuneRequest_timer > 0) && (millis() - tuneRequest_timer > 60)) {
    sr.writePin(TUNE_REQ_LED, LOW);
    tuneRequest_timer = 0;
  }

  if ((clock_timer > 0) && (millis() - clock_timer > 60)) {
    sr.writePin(TIMING_CLOCK_LED, LOW);
    clock_timer = 0;
  }

  if ((sustain_timer > 0) && (millis() - sustain_timer > 60)) {
    sr.writePin(SUSTAIN_LED, LOW);
    sustain_timer = 0;
  }

  if ((channel1_timer > 0) && (millis() - channel1_timer > 60)) {
    sr.writePin(CHANNEL_1_LED, LOW);
    channel1_timer = 0;
  }

  if ((channel2_timer > 0) && (millis() - channel2_timer > 60)) {
    sr.writePin(CHANNEL_2_LED, LOW);
    channel2_timer = 0;
  }

  if ((channel3_timer > 0) && (millis() - channel3_timer > 60)) {
    sr.writePin(CHANNEL_3_LED, LOW);
    channel3_timer = 0;
  }

  if ((channel4_timer > 0) && (millis() - channel4_timer > 60)) {
    sr.writePin(CHANNEL_4_LED, LOW);
    channel4_timer = 0;
  }

  if ((channel5_timer > 0) && (millis() - channel5_timer > 60)) {
    sr.writePin(CHANNEL_5_LED, LOW);
    channel5_timer = 0;
  }

  if ((channel6_timer > 0) && (millis() - channel6_timer > 60)) {
    sr.writePin(CHANNEL_6_LED, LOW);
    channel6_timer = 0;
  }

  if ((channel7_timer > 0) && (millis() - channel7_timer > 60)) {
    sr.writePin(CHANNEL_7_LED, LOW);
    channel7_timer = 0;
  }

  if ((channel8_timer > 0) && (millis() - channel8_timer > 60)) {
    sr.writePin(CHANNEL_8_LED, LOW);
    channel8_timer = 0;
  }

  if ((channel9_timer > 0) && (millis() - channel9_timer > 60)) {
    sr.writePin(CHANNEL_9_LED, LOW);
    channel9_timer = 0;
  }

  if ((channel10_timer > 0) && (millis() - channel10_timer > 60)) {
    sr.writePin(CHANNEL_10_LED, LOW);
    channel10_timer = 0;
  }

  if ((channel11_timer > 0) && (millis() - channel11_timer > 60)) {
    sr.writePin(CHANNEL_11_LED, LOW);
    channel11_timer = 0;
  }

  if ((channel12_timer > 0) && (millis() - channel12_timer > 60)) {
    sr.writePin(CHANNEL_12_LED, LOW);
    channel12_timer = 0;
  }

  if ((channel13_timer > 0) && (millis() - channel13_timer > 60)) {
    sr.writePin(CHANNEL_13_LED, LOW);
    channel13_timer = 0;
  }

  if ((channel14_timer > 0) && (millis() - channel14_timer > 60)) {
    sr.writePin(CHANNEL_14_LED, LOW);
    channel14_timer = 0;
  }

  if ((channel15_timer > 0) && (millis() - channel15_timer > 60)) {
    sr.writePin(CHANNEL_15_LED, LOW);
    channel15_timer = 0;
  }

  if ((channel16_timer > 0) && (millis() - channel16_timer > 60)) {
    sr.writePin(CHANNEL_16_LED, LOW);
    channel16_timer = 0;
  }
}


void channelLED(byte channel) {
  switch (channel) {

    case 1:
      sr.writePin(CHANNEL_1_LED, HIGH);
      channel1_timer = millis();
      break;

    case 2:
      sr.writePin(CHANNEL_2_LED, HIGH);
      channel2_timer = millis();
      break;

    case 3:
      sr.writePin(CHANNEL_3_LED, HIGH);
      channel3_timer = millis();
      break;

    case 4:
      sr.writePin(CHANNEL_4_LED, HIGH);
      channel4_timer = millis();
      break;

    case 5:
      sr.writePin(CHANNEL_5_LED, HIGH);
      channel5_timer = millis();
      break;

    case 6:
      sr.writePin(CHANNEL_6_LED, HIGH);
      channel6_timer = millis();
      break;

    case 7:
      sr.writePin(CHANNEL_7_LED, HIGH);
      channel7_timer = millis();
      break;

    case 8:
      sr.writePin(CHANNEL_8_LED, HIGH);
      channel8_timer = millis();
      break;

    case 9:
      sr.writePin(CHANNEL_9_LED, HIGH);
      channel9_timer = millis();
      break;

    case 10:
      sr.writePin(CHANNEL_10_LED, HIGH);
      channel10_timer = millis();
      break;

    case 11:
      sr.writePin(CHANNEL_11_LED, HIGH);
      channel11_timer = millis();
      break;

    case 12:
      sr.writePin(CHANNEL_12_LED, HIGH);
      channel12_timer = millis();
      break;

    case 13:
      sr.writePin(CHANNEL_13_LED, HIGH);
      channel13_timer = millis();
      break;

    case 14:
      sr.writePin(CHANNEL_14_LED, HIGH);
      channel14_timer = millis();
      break;

    case 15:
      sr.writePin(CHANNEL_15_LED, HIGH);
      channel15_timer = millis();
      break;

    case 16:
      sr.writePin(CHANNEL_16_LED, HIGH);
      channel16_timer = millis();
      break;
  }
}
