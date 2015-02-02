// Copyright 2015 Yasuki IKEUCHI

#include "gtest/gtest.h"
#include "../Protocol.h"

char gMockExpectedChar;
int gMockCharacterRecievedCalledCount = 0;
void mockOnCharacterRecieved(char c) {
  gMockCharacterRecievedCalledCount++;
  ASSERT_EQ(gMockExpectedChar, c);
}

char gMockExpectedError;
int gMockErrorCalledCount = 0;
void mockOnError(int errorCode) {
  gMockErrorCalledCount++;
  ASSERT_EQ(gMockExpectedError, errorCode);
}

TEST(character, normal) {
  Protocol protocol;
  protocol.setOnCharacterRecieved(&mockOnCharacterRecieved);

  gMockCharacterRecievedCalledCount = 0;
  gMockExpectedChar = 'a';
  gMockErrorCalledCount = 0;

  protocol.pushRGB(0b110); // start bit
  ASSERT_EQ(0b1, protocol.getBufferForTest());
  protocol.pushRGB(0b111); //'a' = 97 = 0b1100001
  ASSERT_EQ(0b111, protocol.getBufferForTest());
  protocol.pushRGB(0b001);
  ASSERT_EQ(0b11100, protocol.getBufferForTest());
  protocol.pushRGB(0b001);
  ASSERT_EQ(0b1110000, protocol.getBufferForTest());
  protocol.pushRGB(0b111);
  ASSERT_EQ(1, gMockCharacterRecievedCalledCount);
  ASSERT_EQ(0, gMockErrorCalledCount);
}

TEST(character, parityError) {
  Protocol protocol;
  protocol.setOnCharacterRecieved(&mockOnCharacterRecieved);
  protocol.setOnError(&mockOnError);

  gMockCharacterRecievedCalledCount = 0;
  gMockErrorCalledCount = 0;
  gMockExpectedError = DISPLAY_LIFI_ERROR_PARITY;

  protocol.pushRGB(0b110); // start bit
  ASSERT_EQ(0b1, protocol.getBufferForTest());
  protocol.pushRGB(0b111); //'a' = 97 = 0b1100001
  ASSERT_EQ(0b111, protocol.getBufferForTest());
  protocol.pushRGB(0b001);
  ASSERT_EQ(0b11100, protocol.getBufferForTest());
  protocol.pushRGB(0b001);
  ASSERT_EQ(0b1110000, protocol.getBufferForTest());
  protocol.pushRGB(0b011);
  ASSERT_EQ(0, gMockCharacterRecievedCalledCount);
  ASSERT_EQ(1, gMockErrorCalledCount);
}

