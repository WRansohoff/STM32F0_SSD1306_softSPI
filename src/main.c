#include "main.h"

/**
 * Main program.
 */
int main(void) {
  // Enable the GPIOB peripheral in 'RCC_AHBENR'.
  RCC->AHBENR   |= RCC_AHBENR_GPIOBEN;
  /* Initialize the GPIOB pins.
   * For software sspi with no MISO, use push-pull
   * outputs at high speed for each pin. */
  // Mode: Output
  GPIOB->MODER   &= ~((0x3 << (PB_MOSI * 2)) |
                      (0x3 << (PB_SCK  * 2)) |
                      (0x3 << (PB_DC   * 2)) |
                      (0x3 << (PB_CS   * 2)));
  GPIOB->MODER   |=  ((0x1 << (PB_MOSI * 2)) |
                      (0x1 << (PB_SCK  * 2)) |
                      (0x1 << (PB_DC   * 2)) |
                      (0x1 << (PB_CS   * 2)));
  // Output type: Push-pull
  GPIOB->OTYPER  &= ~((0x1 << PB_MOSI) |
                      (0x1 << PB_SCK)  |
                      (0x1 << PB_DC)   |
                      (0x1 << PB_CS));
  // No pull-up or pull-down resistors.
  GPIOB->PUPDR   &= ~((0x3 << (PB_MOSI * 2)) |
                      (0x3 << (PB_SCK  * 2)) |
                      (0x3 << (PB_DC   * 2)) |
                      (0x3 << (PB_CS   * 2)));
  // High-speed - 50MHz maximum
  // (Setting all '1's, so no need to clear bits first.)
  GPIOB->OSPEEDR |=  ((0x3 << (PB_MOSI * 2)) |
                      (0x3 << (PB_SCK  * 2)) |
                      (0x3 << (PB_DC   * 2)) |
                      (0x3 << (PB_CS   * 2)));

  // Perform SSD1306 panel initialization.
  // Set initial pin values.
  //   (The 'Chip Select' pin tells the display if it
  //    should be listening. '0' means 'hey, listen!', and
  //    '1' means 'ignore the SCK/MOSI/DC pins'.)
  GPIOB->ODR &= ~(1 << PB_CS);
  //   (See the 'sspi_cmd' method for 'DC' pin info.)
  GPIOB->ODR |=  (1 << PB_DC);

  // Send initialization commands for a 128x64 display.
  // TODO: Constants/macros for command values.
  // 'Display off'.
  sspi_cmd(0xAE);
  // 'Set display clock division'
  sspi_cmd(0xD5);
  sspi_cmd(0x80);
  // 'Set multiplexing.' (64-1)
  sspi_cmd(0xA8);
  sspi_cmd(0x3F);
  // 'Set display offset.'
  sspi_cmd(0xD3);
  sspi_cmd(0x00);
  // 'Set start line.' (| with value, 0 here)
  sspi_cmd(0x40 | 0x00);
  // Enable the internal charge pump.
  sspi_cmd(0x8D);
  sspi_cmd(0x14);
  // 'Memory mode.'
  sspi_cmd(0x20);
  sspi_cmd(0x00);
  // 'SEGREMAP'...? (| with value, 1 here)
  sspi_cmd(0xA0 | 0x01);
  // 'COM Scan: Ascending'
  sspi_cmd(0xC0);
  // (or, 'Descending' to flip the display vertically.)
  //sspi_cmd(0xC8);
  // 'COMPINS' value...?
  sspi_cmd(0xDA);
  sspi_cmd(0x12);
  // 'Set Contrast Scaling'
  sspi_cmd(0x81);
  sspi_cmd(0xCF);
  // 'Set Precharge'
  sspi_cmd(0xD9);
  sspi_cmd(0xF1);
  // 'Set VCOM Detect'.
  // (Something to do with the VCOMH pin?)
  sspi_cmd(0xDB);
  sspi_cmd(0x40);
  // 'Display all on resume'
  sspi_cmd(0xA4);
  // 'Normal display mode'
  sspi_cmd(0xA6);
  // 'Deactivate scrolling'
  sspi_cmd(0x2E);
  // 'Display On'
  sspi_cmd(0xAF);

  // (Main loop - draw to the screen.)
  uint16_t px_i = 0;
  while (1) {
    // Draw to the screen.
    // 'Column Address Range'
    sspi_cmd(0x21);
    sspi_cmd(0x00);
    sspi_cmd(127);
    // 'Page Address Range' [page = (row / 8)]
    sspi_cmd(0x22);
    sspi_cmd(0x00);
    sspi_cmd(0x07);
    // Write data.
    for (px_i = 0; px_i < (OLED_BUF_SIZE); ++px_i) {
      sspi_w(oled_buffer[px_i]);
    }
  }
}
