/** @file piece_types.h
 * P/ECE arch-specific structures
 */

#ifndef PIECE_TYPES_H
#define PIECE_TYPES_H

#include "types.h"

// derived from P/ECE SDK
typedef struct systeminfo
{
  c33hword size;         //  0 ���ι�¤�ΤΥ�����
  c33hword hard_ver;     //  2 �ϡ��ɥ��������С������
  c33hword bios_ver;     //  4 BIOS�С������
  c33hword bios_date;    //  6 BIOS������ YY(7):MM(4):DD(5)
  c33word  sys_clock;    //  8 �����ƥࡦ����å�(Hz)
  c33hword vdde_voltage; // 12 VDDE(����)�Ű�(mV)
  c33hword resv1;        // 14 ͽ��
  c33word sram_top;     // 16 SRAM ���ϥ��ɥ쥹
  c33word sram_end;     // 20 SRAM ��λ���ɥ쥹+1
  c33word pffs_top;     // 24 pffs ���ϥ��ɥ쥹
  c33word pffs_end;     // 28 pffs ��λ���ɥ쥹
} SYSTEMINFO;

#endif
