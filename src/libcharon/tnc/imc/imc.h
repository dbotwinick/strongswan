/*
 * Copyright (C) 2010 Andreas Steffen
 * HSR Hochschule fuer Technik Rapperswil
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

/**
 * @defgroup imc imc
 * @{ @ingroup libcharon
 */

#ifndef IMC_H_
#define IMC_H_

#include <tnc/tncifimc.h>

#include <library.h>

typedef struct imc_t imc_t;

struct imc_t {

	/**
	 * The TNC Client calls this function to initialize the IMC and agree on
	 * the API version number to be used. It also supplies the IMC ID, an IMC
	 * identifier that the IMC must use when calling TNC Client callback functions.
	 *
	 * @param imcID				IMC ID assigned by TNCC
	 * @param minVersion		Minimum API version supported by TNCC
	 * @param maxVersion		Maximum API version supported by TNCC
	 * @param OutActualVersion	Mutually supported API version number
	 * @return					TNC result code
	 */
	TNC_Result (*initialize)(TNC_IMCID imcID,
							 TNC_Version minVersion,
							 TNC_Version maxVersion,
							 TNC_Version *OutActualVersion);

	/**
	 * The TNC Client calls this function to inform the IMC that the state of
	 * the network connection identified by connectionID has changed to newState.
	 *
	 * @param imcID				IMC ID assigned by TNCC
	 * @param connectionID		Network connection ID assigned by TNCC
	 * @param newState			New network connection state
	 * @return					TNC result code
	 */
	TNC_Result (*notify_connection_change)(TNC_IMCID imcID,
										   TNC_ConnectionID connectionID,
										   TNC_ConnectionState newState);

	/**
	 * The TNC Client calls this function to indicate that an Integrity Check
	 * Handshake is beginning and solicit messages from IMCs for the first batch.
	 *
	 * @param imcID				IMC ID assigned by TNCC
	 * @param connectionID		Network connection ID assigned by TNCC
	 * @return					TNC result code
	 */
	TNC_Result (*begin_handshake)(TNC_IMCID imcID,
								  TNC_ConnectionID connectionID);

	/**
	 * The TNC Client calls this function to deliver a message to the IMC.
	 * The message is contained in the buffer referenced by message and contains
	 * the number of octets indicated by messageLength. The type of the message
	 * is indicated by messageType.
	 *
	 * @param imcID				IMC ID assigned by TNCS
	 * @param connectionID		Network connection ID assigned by TNCC
	 * @param message			Reference to buffer containing message
	 * @param messageLength		Number of octets in message
	 * @param messageType		Message type of message
	 * @return					TNC result code
	 */
	TNC_Result (*receive_message)(TNC_IMCID imcID,
								  TNC_ConnectionID connectionID,
								  TNC_BufferReference message,
								  TNC_UInt32 messageLength,
								  TNC_MessageType messageType);

	/**
	 * The TNC Client calls this function to notify IMCs that all IMV messages
	 * received in a batch have been delivered and this is the IMC’s last chance
	 * to send a message in the batch of IMC messages currently being collected.
	 *
	 * @param imcID				IMC ID assigned by TNCC
	 * @param connectionID		Network connection ID assigned by TNCC
	 * @return					TNC result code
	 */
	TNC_Result (*batch_ending)(TNC_IMCID imcID,
							   TNC_ConnectionID connectionID);

	/**
	 * The TNC Client calls this function to close down the IMC when all work is
	 * complete or the IMC reports TNC_RESULT_FATAL.
	 *
	 * @param imcID				IMC ID assigned by TNCC
	 * @return					TNC result code
	 */
	TNC_Result (*terminate)(TNC_IMCID imcID);

	/**
	 * IMVs implementing the UNIX/Linux Dynamic Linkage platform binding MUST
	 * define this additional function. The TNC Server MUST call the function
	 * immediately after calling TNC_IMV_Initialize to provide a pointer to the
	 * TNCS bind function. The IMV can then use the TNCS bind function to obtain
	 * pointers to any other TNCS functions.
	 *
	 * @param imcID				IMC ID assigned by TNCC
	 * @param bindFunction		Pointer to TNC_TNCC_BindFunction
	 * @return					TNC result code
	 */
	TNC_Result (*provide_bind_function)(TNC_IMCID imcID,
										TNC_TNCC_BindFunctionPointer bindFunction);

	/**
	 * Sets the ID of an imc_t object.
	 *
	 * @param id				IMC ID to be assigned
	 */
	void (*set_id)(imc_t *this, TNC_IMCID id);

	/**
	 * Returns the ID of an imc_t object.
	 *
	 * @return					assigned IMC ID
	 */
	TNC_IMCID (*get_id)(imc_t *this);

	/**
	 * Returns the name of an imc_t object.
	 *
	 * @return					name of IMC
	 */
	char* (*get_name)(imc_t *this);

	/**
	 * Sets the supported message types of an imc_t object.
	 *
	 * @param supported_types	List of messages type supported by IMC
	 * @param type_count		Number of supported message types
	 */
	void (*set_message_types)(imc_t *this, TNC_MessageTypeList supported_types,
										   TNC_UInt32 type_count);

	/**
	 * Check if the IMC supports a given message type.
	 *
	 * @param message_type		message type
	 * @return					TRUE if supported
	 */
	bool (*type_supported)(imc_t *this, TNC_MessageType message_type);

	/**
	 * Destroys an imc_t object.
	 */
	void (*destroy)(imc_t *this);
};

#endif /** IMC_H_ @}*/
