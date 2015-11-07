#ifndef CLOCK_H
#define CLOCK_H

/**
 * References:
 * http://wiki.nesdev.com/w/index.php/Clock_rate
 * http://wiki.nesdev.com/w/index.php/CPU
 */

#define SUBCARRIER_FREQUENCY 39375000 / 11.0f
#define MASTER_FREQUENCY SUBCARRIER_FREQUENCY * 6.0f
#define CPU_FREQUENCY MASTER_FREQUENCY / 12.0f
#define PPU_FREQUENCY MASTER_FREQUENCY / 4.0f
#define APU_FREQUENCY MASTER_FREQUENCY / 24.0f

/**
 * Calculates the number of cycles that occur in a scaled
 * frequency given the clock of the original frequency
 *
 * Notes:
 *  - The integer part of the ratio must fit into an int
 *  - May lose accuracy if or when the clock overflows
 */
int frequency_scale(float ratio, int clock);

#endif
