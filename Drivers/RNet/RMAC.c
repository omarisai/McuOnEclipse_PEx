/**
 * \file
 * \brief This is the implementation of the Radio MAC part.
 * \author (c) 2013-2014 Erich Styger, http://mcuoneclipse.com/
 * \note MIT License (http://opensource.org/licenses/mit-license.html), see 'RNet_License.txt'
 *
 * This module encapsulates the Media Access of network stack.
 */

#include "RNetConf.h"
#include "RMAC.h"
#include "RPHY.h"
#include "RNWK.h"
#include "%@Utility@'ModuleName'.h"

static uint8_t RMAC_SeqNr = 0;
static uint8_t RMAC_ExpectedAckSeqNr;

uint8_t RMAC_PutPayload(uint8_t *buf, size_t bufSize, uint8_t payloadSize, RPHY_FlagsType flags) {
  if (flags&RPHY_PACKET_FLAGS_REQ_ACK) {
    RMAC_BUF_TYPE(buf) = RMAC_MSG_TYPE_DATA|RMAC_MSG_TYPE_REQ_ACK;
  } else {
    RMAC_BUF_TYPE(buf) = RMAC_MSG_TYPE_DATA;
  }
  RMAC_ExpectedAckSeqNr = RMAC_SeqNr;
  RMAC_BUF_SEQN(buf) = RMAC_SeqNr++;
  return RPHY_PutPayload(buf, bufSize, payloadSize+RMAC_HEADER_SIZE, flags);
}

uint8_t RMAC_OnPacketRx(RPHY_PacketDesc *packet) {
  return RNWK_OnPacketRx(packet); /* pass data packet up the stack */
}

uint8_t RMAC_SendACK(RPHY_PacketDesc *rxPacket, RPHY_PacketDesc *ackPacket) {
  RMAC_BUF_TYPE(ackPacket->phyData) = RMAC_MSG_TYPE_ACK; /* set type to ack */
  RMAC_BUF_SEQN(ackPacket->phyData) = RMAC_BUF_SEQN(rxPacket->phyData);
  /* use same sequence number as in the received package, so no change */
  return RPHY_PutPayload(ackPacket->phyData, ackPacket->phySize, RMAC_HEADER_SIZE+RNWK_HEADER_SIZE, RPHY_PACKET_FLAGS_NONE);
}

RMAC_MsgType RMAC_GetType(uint8_t *buf, size_t bufSize) {
  (void)bufSize; /* not used */
  return RMAC_BUF_TYPE(buf);
}

bool RMAC_IsExpectedACK(uint8_t *buf, size_t bufSize) {
  (void)bufSize; /* not used */
  return RMAC_BUF_SEQN(buf)==RMAC_ExpectedAckSeqNr;
}

void RMAC_SniffPacket(RPHY_PacketDesc *packet, bool isTx) {
  RNWK_SniffPacket(packet, isTx);
}

void RMAC_DecodeType(uint8_t *buf, size_t bufSize, RPHY_PacketDesc *packet) {
  RMAC_MsgType type;
  bool first = TRUE;
  
  type = RMAC_BUF_TYPE(packet->phyData);
  buf[0] = '\0';
  %@Utility@'ModuleName'%.chcat(buf, bufSize, '(');
  if (type&RMAC_MSG_TYPE_REQ_ACK) {
    %@Utility@'ModuleName'%.strcat(buf, bufSize, (unsigned char*)"RACK");
    first = FALSE;
  } else {
    %@Utility@'ModuleName'%.strcat(buf, bufSize, (unsigned char*)"NACK");
    first = FALSE;
  }
  if (type&RMAC_MSG_TYPE_DATA) {
    if (!first) {
      %@Utility@'ModuleName'%.chcat(buf, bufSize, '|');
    }
    %@Utility@'ModuleName'%.strcat(buf, bufSize, (unsigned char*)"DATA");
    first = FALSE;
  }
  if (type&RMAC_MSG_TYPE_ACK) {
    if (!first) {
      %@Utility@'ModuleName'%.chcat(buf, bufSize, '|');
    }
    %@Utility@'ModuleName'%.strcat(buf, bufSize, (unsigned char*)"ACK");
    first = FALSE;
  }
  if (type&RMAC_MSG_TYPE_CMD) {
    %@Utility@'ModuleName'%.strcat(buf, bufSize, (unsigned char*)"CMD");
    first = FALSE;
  }
  %@Utility@'ModuleName'%.chcat(buf, bufSize, ')');
}

void RMAC_Deinit(void) {
  /* nothing needed */
}

void RMAC_Init(void) {
  RMAC_SeqNr = 0;
  RMAC_ExpectedAckSeqNr = 0;
}
