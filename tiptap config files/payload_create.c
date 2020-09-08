/**
 * @brief Calculate HMAC-SHA256 of a string
 *
 * @param *payload : String whose hash has to be calculated
 * @param payloadLength : Length of the input string
 * @param *key : Secret Key
 * @param keyLength : Secret Key length
 * @param *hmacResult : buffer to get hash result in hex format
 *
 * @return None
 *
 */
void Calculate_HMAC_SHA256(uint8_t* payload, size_t payloadLength, uint8_t* key, size_t keyLength, uint8_t* hmacResult)
{
    mbedtls_md_context_t ctx;
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;

    mbedtls_md_init(&ctx);
    mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 1);
    mbedtls_md_hmac_starts(&ctx, (const unsigned char *) key, keyLength);
    mbedtls_md_hmac_update(&ctx, (const unsigned char *) payload, payloadLength);
    mbedtls_md_hmac_finish(&ctx, hmacResult);
    mbedtls_md_free(&ctx);
}

/**
 * @brief Careate JSON payload for processing tarssaction on server from the terminal data
 *
 * @param *terminalData : payment Data received from the terminal
 * @param length : Length of the terminalData
 *
 * @return None
 *
 */
void CreteateJson(uint8_t *terminalData, uint32_t length)
{
	taskENTER_CRITICAL();
	mbedtls_aes_context sAes;
	uint8_t ui8IV[16] = {0};
	uint8_t ui8Input [2048] = {0};
	uint8_t ui8Output[2048] = {0};
	uint8_t ui8Signature[32] = {0};
	uint8_t ui8Pad = 0;
	uint32_t ui32InLength = 0;
	uint32_t ui32OutLength = 0;
	uint8_t ui8Index = 0;

	DbgConsole_Printf( "\r\nKey: ");

	for(ui8Index = 0; ui8Index < sizeof(_pui8AesKey); ui8Index++)
	{
		DbgConsole_Printf("%02X", _pui8AesKey[ui8Index]);
	}

	DbgConsole_Printf("\r\n");

	memcpy(ui8Input, terminalData, length);

	ui32InLength = length;

	ui8Pad = ui32InLength % 16;

	if(ui8Pad)
		ui8Pad = 16 - ui8Pad;

	for(ui8Index = 0; ui8Index < ui8Pad; ui8Index++)
	{
		ui8Input[ui32InLength + ui8Index] = ui8Pad;
	}
	ui32InLength += ui8Pad;

	mbedtls_aes_setkey_enc( &sAes, _pui8AesKey, 256 );

	memset(ui8IV,0,sizeof(ui8IV));
	sprintf((char*)ui8IV, _pcSerialNumber);

	mbedtls_aes_crypt_cbc( &sAes, MBEDTLS_AES_ENCRYPT, ui32InLength, ui8IV, ui8Input, ui8Output);

	sprintf(_pcJson, "{ \"SerialNumber\": \"%s\", \"RSSI\": %d, \"TimeStamp\": %u, \"TransactionID\": \"%s\", \"Payload\": \"", _pcSerialNumber, _i8Rssi, (unsigned int)GetEpochTime(), _pcTRansactionID);

	mbedtls_base64_encode ((unsigned char *)&_pcJson[strlen(_pcJson)], sizeof(_pcJson) - strlen(_pcJson), (size_t*)&ui32OutLength, (unsigned char*)ui8Output, ui32InLength);

	strcat(_pcJson, "\",");

	Calculate_HMAC_SHA256((uint8_t*)_pcJson, strlen(_pcJson), _pui8AesKey, sizeof(_pui8AesKey), ui8Signature);
	strcat(_pcJson, " \"Signature\": \"");
	mbedtls_base64_encode ((unsigned char *)&_pcJson[strlen(_pcJson)], sizeof(_pcJson) - strlen(_pcJson), (size_t*)&ui32OutLength, (unsigned char*)ui8Signature, sizeof(ui8Signature));
	strcat(_pcJson, "\" }");

	DbgConsole_Printf("%s\r\n", _pcJson);

	taskEXIT_CRITICAL();
}
