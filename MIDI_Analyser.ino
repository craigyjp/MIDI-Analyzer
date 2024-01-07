#include <MIDI.h>
#include <RoxMux.h>
#include "Parameters.h"
#include "HWControls.h"
#include <Adafruit_GFX.h>
#include <ST7735_t3.h>
#include <SPI.h>
#include <EEPROM.h>

ST7735_t3 tft = ST7735_t3(2, 3, 11, 13, 21);

#define SR_TOTAL 4
Rox74HC595<SR_TOTAL> sr;

// pins for 74HC595
#define LED_DATA 5   // pin 14 on 74HC595 (DATA)
#define LED_LATCH 7  // pin 12 on 74HC595 (LATCH)
#define LED_CLK 6    // pin 11 on 74HC595 (CLK)
#define LED_PWM -1   // pin 13 on 74HC595

//MIDI 5 Pin DIN
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

RoxButton upButton;
RoxButton dnButton;
RoxButton selButton;

void setup() {

  sr.begin(LED_DATA, LED_LATCH, LED_CLK, LED_PWM);

  tft.initR(INITR_MINI160x80);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  renderBootUpPage();
  delay(500);
  display_timer = millis();
  //renderCurrentPatchPage();

  setupHardware();
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOn(midi::Thru::Mode::Off);

  upButton.begin();
  upButton.setDoublePressThreshold(300);

  dnButton.begin();
  dnButton.setDoublePressThreshold(300);

  selButton.begin();
  selButton.setDoublePressThreshold(500);
}

void displayValue(byte value) {
  switch (value) {
    case 0:
      tft.setCursor(120, 25);
      tft.print("Off");
      break;

    case 1:
      tft.setCursor(120, 25);
      tft.print("On");
      break;
  }
}

void updateParam() {
  switch (paramNumber) {
    case 0:
      note_filter = !note_filter;
      EEPROM.update(EEPROM_NOTES, note_filter);
      break;

    case 1:
      aftertouch_filter = !aftertouch_filter;
      EEPROM.update(EEPROM_AFTERTOUCH, aftertouch_filter);
      break;

    case 2:
      bend_filter = !bend_filter;
      EEPROM.update(EEPROM_PITCHBEND, bend_filter);
      break;

    case 3:
      mod_filter = !mod_filter;
      EEPROM.update(EEPROM_MODULATION, mod_filter);
      break;

    case 4:
      program_filter = !program_filter;
      EEPROM.update(EEPROM_PROGRAM, program_filter);
      break;

    case 5:
      realtime_filter = !realtime_filter;
      EEPROM.update(EEPROM_REALTIME, realtime_filter);
      break;

    case 6:
      system_filter = !system_filter;
      EEPROM.update(EEPROM_SYSTEM, system_filter);
      break;

    case 7:
      controllers_filter = !controllers_filter;
      EEPROM.update(EEPROM_CONTROLLERS, controllers_filter);
      break;

    case 8:
      active_filter = !active_filter;
      EEPROM.update(EEPROM_ACTIVE, active_filter);
      break;

    case 9:
      sustain_filter = !sustain_filter;
      EEPROM.update(EEPROM_SUSTAIN, sustain_filter);
      break;

    case 10:
      all_notes_off_filter = !all_notes_off_filter;
      EEPROM.update(EEPROM_NOTES_OFF, all_notes_off_filter);
      break;
  }
  oldparamNumber = -1;
}


void updateDisplay() {

  if (!sendData) {
    if (oldparamNumber != paramNumber) {

      tft.fillScreen(ST7735_BLACK);
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.setTextColor(ST7735_WHITE);
      tft.print("Filtering");
      tft.setCursor(0, 25);
      tft.setTextColor(ST7735_YELLOW);
      switch (paramNumber) {
        case 0:
          note_filter = EEPROM.read(EEPROM_NOTES);
          if (note_filter < 0 || note_filter > 1) {
            EEPROM.update(EEPROM_NOTES, 0);
          }
          tft.println("Notes");
          displayValue(note_filter);
          break;

        case 1:
          aftertouch_filter = EEPROM.read(EEPROM_AFTERTOUCH);
          if (aftertouch_filter < 0 || aftertouch_filter > 1) {
            EEPROM.update(EEPROM_AFTERTOUCH, 0);
          }
          tft.println("After");
          tft.setCursor(0, 50);
          tft.println("Touch");
          displayValue(aftertouch_filter);
          break;

        case 2:
          bend_filter = EEPROM.read(EEPROM_PITCHBEND);
          if (bend_filter < 0 || bend_filter > 1) {
            EEPROM.update(EEPROM_PITCHBEND, 0);
          }
          tft.println("PitchBend");
          displayValue(bend_filter);
          break;

        case 3:
          mod_filter = EEPROM.read(EEPROM_MODULATION);
          if (mod_filter < 0 || mod_filter > 1) {
            EEPROM.update(EEPROM_MODULATION, 0);
          }
          tft.println("Mod");
          tft.setCursor(0, 50);
          tft.println("Wheel");
          displayValue(mod_filter);
          break;

        case 4:
          program_filter = EEPROM.read(EEPROM_PROGRAM);
          if (program_filter < 0 || program_filter > 1) {
            EEPROM.update(EEPROM_PROGRAM, 0);
          }
          tft.println("Program");
          displayValue(program_filter);
          break;

        case 5:
          realtime_filter = EEPROM.read(EEPROM_REALTIME);
          if (realtime_filter < 0 || realtime_filter > 1) {
            EEPROM.update(EEPROM_REALTIME, 0);
          }
          tft.println("Realtime");
          displayValue(realtime_filter);
          break;

        case 6:
          system_filter = EEPROM.read(EEPROM_SYSTEM);
          if (system_filter < 0 || system_filter > 1) {
            EEPROM.update(EEPROM_SYSTEM, 0);
          }
          tft.println("System");
          displayValue(system_filter);
          break;

        case 7:
          controllers_filter = EEPROM.read(EEPROM_CONTROLLERS);
          if (controllers_filter < 0 || controllers_filter > 1) {
            EEPROM.update(EEPROM_CONTROLLERS, 0);
          }
          tft.println("All");
          tft.setCursor(0, 50);
          tft.println("Controllers");
          displayValue(controllers_filter);
          break;

        case 8:
          active_filter = EEPROM.read(EEPROM_ACTIVE);
          if (active_filter < 0 || active_filter > 1) {
            EEPROM.update(EEPROM_ACTIVE, 0);
          }
          tft.println("Active");
          tft.setCursor(0, 50);
          tft.println("Sense");
          displayValue(active_filter);
          break;

        case 9:
          sustain_filter = EEPROM.read(EEPROM_SUSTAIN);
          if (sustain_filter < 0 || sustain_filter > 1) {
            EEPROM.update(EEPROM_SUSTAIN, 0);
          }
          tft.println("Sustain");
          tft.setCursor(0, 50);
          tft.println("Pedal");
          displayValue(sustain_filter);
          break;

        case 10:
          all_notes_off_filter = EEPROM.read(EEPROM_NOTES_OFF);
          if (all_notes_off_filter < 0 || all_notes_off_filter > 1) {
            EEPROM.update(EEPROM_NOTES_OFF, 0);
          }
          tft.println("All Notes");
          tft.setCursor(0, 50);
          tft.println("Off");
          displayValue(all_notes_off_filter);
          break;
      }
    }
    oldparamNumber = paramNumber;
  }

  if (sendData) {
    if (oldparamNumber != paramNumber) {

      tft.fillScreen(ST7735_BLACK);
      tft.setCursor(0, 0);
      tft.setTextSize(2);
      tft.setTextColor(ST7735_WHITE);
      tft.print("Transmitting");
      tft.setCursor(0, 25);
      tft.setTextColor(ST7735_YELLOW);
      switch (paramNumber) {
        case 0:
          hasValue = false;
          tft.print("Note    ");
          if (data1Select) {
            tft.println(chosenMessage);
          }
          if (sendMessage) {
            MIDI.sendNoteOn(chosenMessage, 127, outChannel);
            sr.writePin(NOTE_ON_LED, HIGH);
            noteOn_timer = millis();
            channelLED(outChannel);
            delay(200);
            MIDI.sendNoteOff(chosenMessage, 0, outChannel);
            sr.writePin(NOTE_OFF_LED, HIGH);
            noteOff_timer = millis();
            channelLED(outChannel);
            sendMessage = false;
          }
          break;

        case 1:
          hasValue = true;
          tft.print("CC Num  ");
          if (data1Select) {
            tft.println(chosenMessage);
          }
          if (data2Select) {
            tft.setCursor(0, 50);
            tft.print("Value   ");
            tft.print(valueMessage);
          }
          if (sendMessage) {
            MIDI.sendControlChange(chosenMessage, valueMessage, outChannel);
            sendMessage = false;
            sr.writePin(CONTROL_LED, HIGH);
            control_timer = millis();
            channelLED(outChannel);
          }
          break;

        case 2:
          hasValue = false;
          tft.print("Prg Chg ");
          if (data1Select) {
            tft.println(chosenMessage);
          }
          if (sendMessage) {
            MIDI.sendProgramChange(chosenMessage, outChannel);
            sendMessage = false;
            digitalWrite(PROGRAM_LED, HIGH);
            program_timer = millis();
            channelLED(outChannel);
          }
          break;

        case 3:
          hasValue = false;
          tft.print("Pressure ");
          if (data1Select) {
            tft.println(chosenMessage);
          }
          if (sendMessage) {
            MIDI.sendAfterTouch(chosenMessage, outChannel);
            sendMessage = false;
            sr.writePin(CHANNEL_PRESSURE_LED, HIGH);
            channelPressure_timer = millis();
            channelLED(outChannel);
          }
          break;

        case 4:
          hasValue = false;
          tft.print("Set Chan ");
          if (data1Select) {
            tft.println(chosenMessage);
          }
          if (sendMessage) {
            outChannel = chosenMessage; 
            sendMessage = false; 
            tft.setCursor(0, 50);
            tft.print("Set");
          }
          break;
      }
    }
    oldparamNumber = paramNumber;
  }
}

void checkSwitches() {

  upButton.update(digitalRead(BUTTON_3), 50, LOW);
  if (upButton.pressed()) {
    if (!sendData) {
      paramNumber++;
      if (paramNumber > 10) {
        paramNumber = 0;
      }
    }

    if (sendData && !data1Select) {
      paramNumber++;
      if (paramNumber > 4) {
        paramNumber = 0;
      }
    }

    if (sendData && data1Select && !data2Select) {
      chosenMessage++;
      if (paramNumber == 4) {
        if (chosenMessage > 16) {
          chosenMessage = 1;
        }
      } else {
        if (chosenMessage > 127) {
          chosenMessage = 0;
        }
      }
      oldparamNumber = -1;
    }

    if (sendData && data1Select && data2Select) {
      valueMessage++;
      if (valueMessage > 127) {
        valueMessage = 0;
      }
      oldparamNumber = -1;
    }
    display_timer = millis();
  }

  dnButton.update(digitalRead(BUTTON_2), 50, LOW);
  if (dnButton.pressed()) {
    if (!sendData) {
      paramNumber--;
      if (paramNumber < 0) {
        paramNumber = 10;
      }
    }

    if (sendData && !data1Select) {
      paramNumber--;
      if (paramNumber < 0) {
        paramNumber = 4;
      }
    }

    if (sendData && data1Select && !data2Select) {
      chosenMessage--;
      if (paramNumber == 4) {
        if (chosenMessage < 1) {
          chosenMessage = 16;
        }
      } else {
        if (chosenMessage < 0) {
          chosenMessage = 127;
        }
      }
      oldparamNumber = -1;
    }

    if (sendData && data1Select && data2Select) {
      valueMessage--;
      if (valueMessage < 0) {
        valueMessage = 127;
      }
      oldparamNumber = -1;
    }
    display_timer = millis();
  }

  selButton.update(digitalRead(BUTTON_1), 50, LOW);
  if (selButton.held()) {
    sendData = !sendData;
    paramNumber = 0;
    display_timer = millis();
    oldparamNumber = -1;
    if (data1Select) {
      data1Select = !data1Select;
      sendData = true;
    }
    if (data2Select) {
      data2Select = !data2Select;
    }

  } else if (selButton.released(true)) {

    if (!sendData) {
      display_timer = millis();
      updateParam();
    }

    if ((sendData && data1Select && data2Select && hasValue) || (sendData && data1Select && !data2Select && !hasValue)) {
      sendMessage = true;
      display_timer = millis();
      oldparamNumber = -1;
    }

    if (sendData && data1Select && data2Select) {
      sendMessage = true;
      display_timer = millis();
      oldparamNumber = -1;
    }

    if (sendData && data1Select && !data2Select && hasValue) {
      data2Select = true;
      display_timer = millis();
      oldparamNumber = -1;
    }

    if (sendData && !data1Select && !data2Select) {
      data1Select = true;
      display_timer = millis();
      oldparamNumber = -1;
    }
  }
}

void displayIncoming(byte type, int16_t data1, int data2, byte channel) {

  //if (refresh_screen) {
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_WHITE);

  tft.setCursor(0, 0);
  tft.print("Note On");
  tft.setCursor(84, 0);
  tft.print("Velocity");

  tft.setCursor(0, 13);
  tft.print("Note Off");
  tft.setCursor(84, 13);
  tft.print("Velocity");

  tft.setCursor(0, 26);
  tft.print("PitchBend");

  tft.setCursor(0, 39);
  tft.print("CC Message");
  tft.setCursor(91, 39);
  tft.print("Value");

  tft.setCursor(0, 52);
  tft.print("PGM Change");

  tft.setCursor(0, 65);
  tft.print("Pressure");
  //}
  refresh_screen = false;


  tft.setTextColor(ST7735_YELLOW);

  switch (type) {
    case midi::NoteOff:
      tft.setTextColor(ST7735_YELLOW);
      tft.setCursor(60, 0);
      tft.print(data1);
      tft.setCursor(145, 0);
      tft.print(data2);
      break;

    case midi::NoteOn:
      tft.setCursor(60, 13);
      tft.print(data1);
      tft.setCursor(145, 13);
      tft.print(data2);
      break;

    case midi::ControlChange:
      tft.setTextColor(ST7735_WHITE);
      switch (data1) {
        case 1:
          tft.setTextColor(ST7735_WHITE);
          tft.setCursor(0, 39);
          break;

        case 64:
          tft.setTextColor(ST7735_WHITE);
          tft.setCursor(0, 39);
          break;

        case 123:
          tft.setTextColor(ST7735_WHITE);
          tft.setCursor(0, 39);
          break;

        default:
          tft.setTextColor(ST7735_WHITE);
          tft.setCursor(0, 39);
          break;
      }
      tft.setTextColor(ST7735_YELLOW);
      tft.setCursor(70, 39);
      tft.print(data1);
      tft.setCursor(135, 39);
      tft.print(data2);
      break;

    case midi::ProgramChange:
      tft.setCursor(70, 52);
      tft.print(data1);
      break;

    case midi::AfterTouchChannel:
      tft.setCursor(70, 65);
      tft.print(data1);
      break;

    case midi::PitchBend:
      tft.setCursor(70, 26);
      //tft.print("Value ");
      tft.print(data1);
      break;
  }
  display_timer = millis();
}

void loop() {

  bool activity = false;

  if (MIDI.read()) {
    // get a MIDI IN1 (Serial) message
    byte type = MIDI.getType();
    byte channel = MIDI.getChannel();
    byte data1 = MIDI.getData1();
    byte data2 = MIDI.getData2();

    switch (type) {
      case midi::NoteOff:  // 0x80
        if (!note_filter) {
          MIDI.send(MIDI.getType(), MIDI.getData1(), MIDI.getData2(), MIDI.getChannel());
        }
        sr.writePin(NOTE_OFF_LED, HIGH);
        noteOff_timer = millis();
        channelLED(channel);
        displayIncoming(type, data1, data2, channel);
        break;

      case midi::NoteOn:  // 0x90
        if (!note_filter) {
          MIDI.send(MIDI.getType(), MIDI.getData1(), MIDI.getData2(), MIDI.getChannel());
        }
        sr.writePin(NOTE_ON_LED, HIGH);
        noteOn_timer = millis();
        channelLED(channel);
        displayIncoming(type, data1, data2, channel);
        break;

      case midi::AfterTouchPoly:  // 0xA0
        sr.writePin(POLY_PRESSURE_LED, HIGH);
        polyPressure_timer = millis();
        channelLED(channel);
        break;

      case midi::ControlChange:  // 0xB0
        switch (data1) {

          case 123:
            if (!all_notes_off_filter) {
              MIDI.sendControlChange(data1, data2, channel);
            }
            sr.writePin(ALL_NOTES_OFF_LED, HIGH);
            allNotes_timer = millis();
            channelLED(channel);
            displayIncoming(type, data1, data2, channel);
            break;

          case 64:
            if (!sustain_filter) {
            }
            sr.writePin(SUSTAIN_LED, HIGH);
            sustain_timer = millis();
            channelLED(channel);
            displayIncoming(type, data1, data2, channel);
            break;

          case 1:
            if (!mod_filter) {
              MIDI.sendControlChange(data1, data2, channel);
            }
            digitalWrite(MODULATION_LED, HIGH);
            modulation_timer = millis();
            channelLED(channel);
            displayIncoming(type, data1, data2, channel);
            break;

          default:
            if (!controllers_filter) {
              MIDI.sendControlChange(data1, data2, channel);
            }
            sr.writePin(CONTROL_LED, HIGH);
            control_timer = millis();
            channelLED(channel);
            displayIncoming(type, data1, data2, channel);
            break;
        }
        break;

      case midi::ProgramChange:  // 0xC0
        if (!program_filter) {
          MIDI.sendProgramChange(data1, channel);
        }
        digitalWrite(PROGRAM_LED, HIGH);
        program_timer = millis();
        channelLED(channel);
        displayIncoming(type, data1, 0, channel);
        break;

      case midi::AfterTouchChannel:  // 0xD0
        if (!aftertouch_filter) {
          MIDI.sendAfterTouch(data1, channel);
        }
        sr.writePin(CHANNEL_PRESSURE_LED, HIGH);
        channelPressure_timer = millis();
        channelLED(channel);
        displayIncoming(type, data1, 0, channel);
        break;

      case midi::PitchBend:  // 0xE0
        pitchBendValue = (data2 << 7) | data1;
        signedPitchBendValue = pitchBendValue - 8192;
        if (!bend_filter) {
          MIDI.sendPitchBend(signedPitchBendValue, channel);
        }
        sr.writePin(PITCHBEND_LED, HIGH);
        pitchbend_timer = millis();
        channelLED(channel);
        displayIncoming(type, signedPitchBendValue, 0, channel);
        break;

      case midi::SystemExclusive:  // 0xF0
        sr.writePin(SYS_EXC_LED, HIGH);
        sysex_timer = millis();
        channelLED(channel);
        break;

      case midi::SongPosition:  // 0xF2
        if (!system_filter) {
          MIDI.sendSongPosition(data1);
        }
        sr.writePin(SONG_POS_LED, HIGH);
        songpos_timer = millis();
        break;

      case midi::SongSelect:  // 0xF3
        if (!system_filter) {
          MIDI.sendSongSelect(data1);
        }
        sr.writePin(SONG_SEL_LED, HIGH);
        songselect_timer = millis();
        break;

      case midi::TuneRequest:  // 0xF6
        if (!system_filter) {
          MIDI.sendTuneRequest();
        }
        sr.writePin(TUNE_REQ_LED, HIGH);
        tuneRequest_timer = millis();
        break;

      case midi::Clock:  // 0xF8
        if (!realtime_filter) {
          MIDI.sendRealTime(MIDI.getType());
        }
        sr.writePin(TIMING_CLOCK_LED, HIGH);
        clock_timer = millis();
        break;

      case midi::Start:  // 0xFA
        if (!realtime_filter) {
          MIDI.sendRealTime(MIDI.getType());
        }
        sr.writePin(START_LED, HIGH);
        songstart_timer = millis();
        break;

      case midi::Continue:  // 0xFB
        if (!realtime_filter) {
          MIDI.sendRealTime(MIDI.getType());
        }
        sr.writePin(CONTINUE_LED, HIGH);
        songcontinue_timer = millis();
        break;

      case midi::Stop:  // 0xFC
        if (!realtime_filter) {
          MIDI.sendRealTime(MIDI.getType());
        }
        sr.writePin(STOP_LED, HIGH);
        songstop_timer = millis();
        break;

      case midi::ActiveSensing:  // 0xFE
        if (!active_filter) {
          MIDI.sendRealTime(MIDI.getType());
        }
        digitalWrite(ACT_SENSE_LED, HIGH);
        activeSensing_timer = millis();
        break;

      case midi::SystemReset:  // 0xFF
        if (!realtime_filter) {
          MIDI.sendRealTime(MIDI.getType());
        }
        digitalWrite(SYSTEM_RESET_LED, HIGH);
        systemReset_timer = millis();
        break;

      default:
        digitalWrite(ERROR_LED, HIGH);
        error_timer = millis();
    }
    activity = true;
  }

  if (usbMIDI.read()) {
    // get a MIDI IN1 (Serial) message
    byte type = usbMIDI.getType();
    byte channel = usbMIDI.getChannel();
    byte data1 = usbMIDI.getData1();
    byte data2 = usbMIDI.getData2();


    switch (type) {
      case midi::NoteOff:  // 0x80
        if (!note_filter) {
          MIDI.sendNoteOff(data1, data2, channel);
        }
        sr.writePin(NOTE_OFF_LED, HIGH);
        noteOff_timer = millis();
        channelLED(channel);
        displayIncoming(type, data1, data2, channel);
        break;

      case midi::NoteOn:  // 0x90
        if (!note_filter) {
          MIDI.sendNoteOn(data1, data2, channel);
        }
        sr.writePin(NOTE_ON_LED, HIGH);
        noteOn_timer = millis();
        channelLED(channel);
        displayIncoming(type, data1, data2, channel);
        break;

      case midi::AfterTouchPoly:  // 0xA0
        //MIDI.sendPolyPressure(data1, data2, channel);
        sr.writePin(POLY_PRESSURE_LED, HIGH);
        polyPressure_timer = millis();
        channelLED(channel);
        break;

      case midi::ControlChange:  // 0xB0
        switch (data1) {

          case 123:
            if (!all_notes_off_filter) {
              MIDI.sendControlChange(data1, data2, channel);
            }
            sr.writePin(ALL_NOTES_OFF_LED, HIGH);
            allNotes_timer = millis();
            channelLED(channel);
            displayIncoming(type, data1, data2, channel);
            break;

          case 64:
            if (!sustain_filter) {
              MIDI.sendControlChange(data1, data2, channel);
            }
            sr.writePin(SUSTAIN_LED, HIGH);
            sustain_timer = millis();
            channelLED(channel);
            displayIncoming(type, data1, data2, channel);
            break;

          case 1:
            if (!mod_filter) {
              MIDI.sendControlChange(data1, data2, channel);
            }
            digitalWrite(MODULATION_LED, HIGH);
            modulation_timer = millis();
            channelLED(channel);
            displayIncoming(type, data1, data2, channel);
            break;

          default:
            if (!controllers_filter) {
              MIDI.sendControlChange(data1, data2, channel);
            }
            sr.writePin(CONTROL_LED, HIGH);
            control_timer = millis();
            channelLED(channel);
            displayIncoming(type, data1, data2, channel);
            break;
        }
        break;

      case midi::ProgramChange:  // 0xC0
        if (!program_filter) {
          MIDI.sendProgramChange(data1, channel);
        }
        digitalWrite(PROGRAM_LED, HIGH);
        program_timer = millis();
        channelLED(channel);
        displayIncoming(type, data1, 0, channel);
        break;

      case midi::AfterTouchChannel:  // 0xD0
        if (!aftertouch_filter) {
          MIDI.sendAfterTouch(data1, channel);
        }
        sr.writePin(CHANNEL_PRESSURE_LED, HIGH);
        channelPressure_timer = millis();
        channelLED(channel);
        displayIncoming(type, data1, 0, channel);
        break;

      case midi::PitchBend:  // 0xE0
        pitchBendValue = (data2 << 7) | data1;
        signedPitchBendValue = pitchBendValue - 8192;
        if (!bend_filter) {
          MIDI.sendPitchBend(signedPitchBendValue, channel);
        }
        sr.writePin(PITCHBEND_LED, HIGH);
        pitchbend_timer = millis();
        channelLED(channel);
        displayIncoming(type, signedPitchBendValue, 0, channel);
        break;

      case midi::SystemExclusive:  // 0xF0
        sr.writePin(SYS_EXC_LED, HIGH);
        sysex_timer = millis();
        channelLED(channel);
        break;

      case midi::SongPosition:  // 0xF2
        if (!system_filter) {
          MIDI.sendSongPosition(data1);
        }
        sr.writePin(SONG_POS_LED, HIGH);
        songpos_timer = millis();
        break;

      case midi::SongSelect:  // 0xF3
        if (!system_filter) {
          MIDI.sendSongSelect(data1);
        }
        sr.writePin(SONG_SEL_LED, HIGH);
        songselect_timer = millis();
        break;

      case midi::TuneRequest:  // 0xF6
        if (!system_filter) {
          MIDI.sendTuneRequest();
        }
        sr.writePin(TUNE_REQ_LED, HIGH);
        tuneRequest_timer = millis();
        break;

      case midi::Clock:  // 0xF8
        //MIDI.sendRealTime(usbMIDI.getType());
        sr.writePin(TIMING_CLOCK_LED, HIGH);
        clock_timer = millis();
        break;

      case midi::Start:  // 0xFA
        //MIDI.sendRealTime(usbMIDI.getType());
        sr.writePin(START_LED, HIGH);
        songstart_timer = millis();
        break;

      case midi::Continue:  // 0xFB
        //MIDI.sendRealTime(usbMIDI.getType());
        sr.writePin(CONTINUE_LED, HIGH);
        songcontinue_timer = millis();
        break;

      case midi::Stop:  // 0xFC
        //MIDI.sendRealTime(usbMIDI.getType());
        sr.writePin(STOP_LED, HIGH);
        songstop_timer = millis();
        break;

      case midi::ActiveSensing:  // 0xFE
        //MIDI.sendRealTime(usbMIDI.getType());
        digitalWrite(ACT_SENSE_LED, HIGH);
        activeSensing_timer = millis();
        break;

      case midi::SystemReset:  // 0xFF
        //MIDI.sendRealTime(usbMIDI.getType());
        digitalWrite(SYSTEM_RESET_LED, HIGH);
        systemReset_timer = millis();
        break;

      default:
        digitalWrite(ERROR_LED, HIGH);
        error_timer = millis();
    }
    activity = true;
  }

  sr.update();
  ledsOff();
  checkSwitches();
  updateDisplay();
  //renderCurrentPatchPage();
  // blink the LED when any activity has happened
  if (activity) {
    digitalWriteFast(MIDI_LED, HIGH);  // LED on
    ledOnMillis = 0;
  }
  if (ledOnMillis > 15) {
    digitalWriteFast(MIDI_LED, LOW);  // LED off
  }
}

void renderBootUpPage() {
  tft.fillScreen(ST7735_BLACK);
  tft.drawRect(0, 0, 160, 60, ST7735_WHITE);
  //tft.fillRect(88, 30, 61, 11, ST7735_WHITE);
  tft.setCursor(3, 10);
  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.println("MIDI ANALYSER");
  tft.setCursor(3, 35);
  tft.setTextSize(2);
  tft.print("& FILTER ");
  tft.setTextColor(ST7735_RED);
  tft.println(VERSION);
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

  if ((error_timer > 0) && (millis() - error_timer > 60)) {
    digitalWrite(ERROR_LED, LOW);
    error_timer = 0;
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

  if ((display_timer > 0) && (millis() - display_timer > 2000)) {
    tft.fillScreen(ST7735_BLACK);
    refresh_screen = true;
    display_timer = 0;
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
