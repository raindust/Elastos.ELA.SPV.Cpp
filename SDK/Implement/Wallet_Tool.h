// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_WALLET_TOOL_H__
#define __ELASTOS_SDK_WALLET_TOOL_H__

#include "CMemBlock.h"

namespace Elastos {
	namespace SDK {
		class Wallet_Tool {
		private:
			static CMemBlock<char>
			_Code_Convert(const std::string from_charset, const std::string to_charset, const CMemBlock<char> &input);

			static CMemBlock<uint8_t> GetRandom(size_t bits);

		public:
			static CMemBlock<char> U8ToU16(const CMemBlock<char> &input);

			static CMemBlock<char> U16ToU8(const CMemBlock<char> &input);

			static CMemBlock<uint8_t> GenerateSeed128();

			static CMemBlock<uint8_t> GenerateSeed256();

			static std::string GenerateEntropySource(const CMemBlock<uint8_t> &seed);

			static std::string getDeriveKeyFromEntropySource_base58(const std::string &seed);

			static std::string getStringFromSeed(const CMemBlock<uint8_t> &seed);

			static CMemBlock<uint8_t> getSeedFromString(const std::string &str_seed);

			static CMemBlock<char> GeneratePhraseFromSeed(const CMemBlock<uint8_t> &seed, const std::string &language);

			static bool PhraseIsValid(const CMemBlock<char> &phrase, const std::string &language);

			static CMemBlock<uint8_t> getSeedFromPhrase(const CMemBlock<char> &phrase, const std::string &language);

			static std::string getDeriveKey_base58(const CMemBlock<char> &phrase, const std::string &passphrase);
		};
	}
}

#endif //__ELASTOS_SDK_WALLET_TOOL_H__