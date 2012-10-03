/*
 EEPROM routines to save settings 
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/

#include <avr/eeprom.h>
#include <bsp/pgmspace.h>
#include <inttypes.h>

#include "makibox.h"
#include "store_eeprom.h"
#include "config.h"
#include "serial.h"

#ifdef PIDTEMP
 extern unsigned int PID_Kp, PID_Ki, PID_Kd;
#endif


#ifdef USE_EEPROM_SETTINGS

//======================================================================================
//========================= Read / Write EEPROM =======================================
template <class T> int EEPROM_write_setting(int address, const T& value)
{
  const byte* p = (const byte*)(const void*)&value;
  int i;
  for (i = 0; i < (int)sizeof(value); i++)
    eeprom_write_byte((unsigned char *)address++, *p++);
  return i;
}

template <class T> int EEPROM_read_setting(int address, T& value)
{
  byte* p = (byte*)(void*)&value;
  int i;
  for (i = 0; i < (int)sizeof(value); i++)
    *p++ = eeprom_read_byte((unsigned char *)address++);
  return i;
}
//======================================================================================


void EEPROM_StoreSettings() 
{
  char ver[4]= "000";
  EEPROM_write_setting(EEPROM_OFFSET, ver); // invalidate data first
  EEPROM_write_setting(axis_steps_per_unit_address, axis_steps_per_unit);
  EEPROM_write_setting(max_feedrate_address, max_feedrate);
  EEPROM_write_setting(max_acceleration_units_per_sq_second_address, max_acceleration_units_per_sq_second);
  EEPROM_write_setting(move_acceleration_address, move_acceleration);
  EEPROM_write_setting(retract_acceleration_address, retract_acceleration);
  EEPROM_write_setting(minimumfeedrate_address, minimumfeedrate);
  EEPROM_write_setting(mintravelfeedrate_address, mintravelfeedrate);
  EEPROM_write_setting(min_seg_time_address, min_seg_time);  //Min Segment Time, not used yet
  EEPROM_write_setting(max_xy_jerk_address, max_xy_jerk);
  EEPROM_write_setting(max_z_jerk_address, max_z_jerk);
  EEPROM_write_setting(max_e_jerk_address, max_e_jerk);

  //PID Settings
  #ifdef PIDTEMP
   EEPROM_write_setting(Kp_address, PID_Kp);     //Kp
   EEPROM_write_setting(Ki_address, PID_Ki);     //Ki
   EEPROM_write_setting(Kd_address, PID_Kd);     //Kd
  #else
   EEPROM_write_setting(Kp_address, 2048);     //Kp
   EEPROM_write_setting(Ki_address, 32);     //Ki
   EEPROM_write_setting(Kd_address, 2048);     //Kd
  #endif
  

  char ver2[4]=EEPROM_VERSION;
  EEPROM_write_setting(EEPROM_OFFSET, ver2); // validate data
  serial_send("Settings Stored\r\n");
 
}


void EEPROM_printSettings()
{  
  #ifdef PRINT_EEPROM_SETTING
      serial_send("Steps per unit:\r\n  M92 X%f Y%f Z%f E%f\r\n",
        axis_steps_per_unit[0],
        axis_steps_per_unit[1],
        axis_steps_per_unit[2],
        axis_steps_per_unit[3]
      );
      
      serial_send("Maximum feedrates (mm/s):\r\n  M202 X%f Y%f Z%f E%f\r\n",
        max_feedrate[0],
        max_feedrate[1],
        max_feedrate[2],
        max_feedrate[3]
      );

      serial_send("Maximum Acceleration (mm/s2):\r\n  M201 X%f Y%f Z%f E%f\r\n",
        max_acceleration_units_per_sq_second[0],
        max_acceleration_units_per_sq_second[1],
        max_acceleration_units_per_sq_second[2],
        max_acceleration_units_per_sq_second[3]
      );

      serial_send("Acceleration: S=acceleration, T=retract acceleration\r\n")
      serial_send("  M204 S%d T%d\r\n", move_acceleration, retract_acceleration);

      serial_send("Advanced variables (mm/s): S=Min feedrate, T=Min travel feedrate, X=max xY jerk,  Z=max Z jerk, E=max E jerk\r\n");
      serial_send("  M205 S%d T%d X%d Z%d E%d\r\n",
        minimumfeedrate,
        mintravelfeedrate,
        max_xy_jerk,
        max_z_jerk,
        max_e_jerk
      );

    #ifdef PIDTEMP
      serial_send("PID settings:\r\n  M301 P%d I%d D%d\r\n", PID_Kp, PID_Ki, PID_Kd); 
    #endif
  #endif

} 


void EEPROM_RetrieveSettings(bool def, bool printout)
{  // if def=true, the default values will be used

    int i=EEPROM_OFFSET;
    char stored_ver[4];
    char ver[4]=EEPROM_VERSION;
    
    EEPROM_read_setting(EEPROM_OFFSET,stored_ver); //read stored version
    if ((!def)&&(strncmp(ver,stored_ver,3)==0))
    {   // version number match
      EEPROM_read_setting(axis_steps_per_unit_address, axis_steps_per_unit);
      EEPROM_read_setting(max_feedrate_address, max_feedrate);
      EEPROM_read_setting(max_acceleration_units_per_sq_second_address, max_acceleration_units_per_sq_second);
      EEPROM_read_setting(move_acceleration_address, move_acceleration);
      EEPROM_read_setting(retract_acceleration_address, retract_acceleration);
      EEPROM_read_setting(minimumfeedrate_address, minimumfeedrate);
      EEPROM_read_setting(mintravelfeedrate_address, mintravelfeedrate);
      EEPROM_read_setting(min_seg_time_address, min_seg_time);  //min Segmenttime --> not used yet
      EEPROM_read_setting(max_xy_jerk_address, max_xy_jerk);
      EEPROM_read_setting(max_z_jerk_address, max_z_jerk);
      EEPROM_read_setting(max_e_jerk_address, max_e_jerk);

      #ifdef PIDTEMP
       EEPROM_read_setting(Kp_address, PID_Kp);
       EEPROM_read_setting(Ki_address, PID_Ki);
       EEPROM_read_setting(Kd_address, PID_Kd);
      #endif

      serial_send("Stored settings retreived\r\n");
    }
    else 
    {

      float tmp1[]=_AXIS_STEP_PER_UNIT;
      float tmp2[]=_MAX_FEEDRATE;
      long tmp3[]=_MAX_ACCELERATION_UNITS_PER_SQ_SECOND;
      for (short i=0;i<4;i++) 
      {
        axis_steps_per_unit[i]=tmp1[i];  
        max_feedrate[i]=tmp2[i];  
        max_acceleration_units_per_sq_second[i]=tmp3[i];
      }
      move_acceleration=_ACCELERATION;
      retract_acceleration=_RETRACT_ACCELERATION;
      minimumfeedrate=DEFAULT_MINIMUMFEEDRATE;
      mintravelfeedrate=DEFAULT_MINTRAVELFEEDRATE;
      max_xy_jerk=_MAX_XY_JERK;
      max_z_jerk=_MAX_Z_JERK;
      max_e_jerk=_MAX_E_JERK;
      min_seg_time=_MIN_SEG_TIME;
      
      #ifdef PIDTEMP
       PID_Kp = PID_PGAIN;
       PID_Ki = PID_IGAIN;
       PID_Kd = PID_DGAIN;
      #endif

      serial_send("Using Default settings\r\n");
    }
    
    if(printout)
    {
      EEPROM_printSettings();
    }
}  

#endif
