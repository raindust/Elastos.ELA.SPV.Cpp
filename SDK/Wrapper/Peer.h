// Copyright (c) 2012-2018 The Elastos Open Source Project
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef __ELASTOS_SDK_PEER_H__
#define __ELASTOS_SDK_PEER_H__

#include <deque>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <SDK/Common/Log.h>

#include "BRPeerMessages.h"

#include "Wrapper.h"
#include "CMemBlock.h"
#include "PeerCallbackInfo.h"
#include "Message/Message.h"
#include "Transaction/ElementSet.h"

#define MSG_VERSION     "version"
#define MSG_VERACK      "verack"
#define MSG_ADDR        "addr"
#define MSG_INV         "inv"
#define MSG_GETDATA     "getdata"
#define MSG_NOTFOUND    "notfound"
#define MSG_GETBLOCKS   "getblocks"
#define MSG_GETHEADERS  "getheaders"
#define MSG_TX          "tx"
#define MSG_BLOCK       "block"
#define MSG_HEADERS     "headers"
#define MSG_GETADDR     "getaddr"
#define MSG_MEMPOOL     "mempool"
#define MSG_PING        "ping"
#define MSG_PONG        "pong"
#define MSG_FILTERLOAD  "filterload"
#define MSG_FILTERADD   "filteradd"
#define MSG_FILTERCLEAR "filterclear"
#define MSG_MERKLEBLOCK "merkleblock"
#define MSG_ALERT       "alert"
#define MSG_REJECT      "reject"   // described in BIP61: https://github.com/bitcoin/bips/blob/master/bip-0061.mediawiki
#define MSG_FEEFILTER   "feefilter"// described in BIP133 https://github.com/bitcoin/bips/blob/master/bip-0133.mediawiki

#ifndef MSG_NOSIGNAL   // linux based systems have a MSG_NOSIGNAL send flag, useful for supressing SIGPIPE signals
#define MSG_NOSIGNAL 0 // set to 0 if undefined (BSD has the SO_NOSIGPIPE sockopt, and windows has no signals at all)
#endif

namespace Elastos {
	namespace ElaWallet {

		class PeerManager;

		class Peer {
		public:
			enum ConnectStatus {
				Disconnected = 0,
				Connecting = 1,
				Connected = 2,
				Unknown = -2
			};

			struct PeerInfo {
				UInt128 address; // IPv6 address of peer
				uint16_t port; // port number for peer connection
				uint64_t services; // bitcoin network services supported by peer
				uint64_t timestamp; // timestamp reported by peer
				uint8_t flags; // scratch variable
			};

			class Listener {
			public:
				virtual void OnConnected(Peer *peer) {}

				virtual void OnDisconnected(Peer *peer, int error) {}

				virtual void OnRelayedPeers(Peer *peer, const std::vector<boost::shared_ptr<Peer>> &peers, size_t peersCount) {}

				virtual void OnRelayedTx(Peer *peer, const TransactionPtr &tx) {}

				virtual void OnHasTx(Peer *peer, const UInt256 &txHash) {}

				virtual void OnRejectedTx(Peer *peer, const UInt256 &txHash, uint8_t code) {}

				virtual void OnRelayedBlock(Peer *peer, const MerkleBlockPtr &block) {}

				virtual void OnRelayedPingMsg(Peer *peer) {}

				virtual void OnNotfound(Peer *peer, const std::vector<UInt256> &txHashes, const std::vector<UInt256> &blockHashes) {}

				virtual void OnSetFeePerKb(Peer *peer, uint64_t feePerKb) {}

				virtual const TransactionPtr &OnRequestedTx(Peer *peer, const UInt256 &txHash) { return nullptr;}

				virtual bool OnNetworkIsReachable(Peer *peer) { return false;}

				virtual void OnThreadCleanup(Peer *peer) {}
			};

			typedef boost::function<void(int)> PeerCallback;

		public:
			Peer(PeerManager *manager, const UInt128 &addr, uint16_t port, uint64_t timestamp);

			Peer(PeerManager *manager, const UInt128 &addr, uint16_t port, uint64_t timestamp, uint64_t services);

			Peer(PeerManager *manager, uint32_t magicNumber);

			Peer(const Peer &peer);

			~Peer();

			Peer &operator=(const Peer &peer);

			void RegisterListner(Listener *listener);

			void UnRegisterListener();

			void SendMessage(const std::string &msgType, const SendMessageParameter &parameter);

			UInt128 getAddress() const;

			void setAddress(const UInt128 &addr);

			uint16_t getPort() const;

			void setPort(uint16_t port);

			uint64_t getTimestamp() const;

			void setTimestamp(uint64_t timestamp);

			uint64_t getServices() const;

			void setServices(uint64_t services);

			void setEarliestKeyTime(uint32_t earliestKeyTime);

			uint32_t GetCurrentBlockHeight() const;

			void SetCurrentBlockHeight(uint32_t currentBlockHeight);

			ConnectStatus getConnectStatusValue() const;

			void Connect();

			void Disconnect();

			void SendMessage(const CMBlock &message, const std::string &type);

			void scheduleDisconnect(double time);

			bool NeedsFilterUpdate() const;

			void SetNeedsFilterUpdate(bool needsFilterUpdate);

			const std::string &getHost() const;

			uint32_t getVersion() const;

			const std::string &getUserAgent() const;

			uint32_t getLastBlock() const;

			uint64_t getFeePerKb() const;

			double getPingTime() const;

			bool IsEqual(const Peer *peer) const;

			bool SentVerack();

			void SetSentVerack(bool sent);

			bool GotVerack();

			void SetGotVerack(bool got);

			bool SentGetaddr();

			void SetSentGetaddr(bool sent);

			bool SentFilter();

			void SetSentFilter(bool sent);

			bool SentGetdata();

			void SetSentGetdata(bool sent);

			bool SentMempool();

			void SetSentMempool(bool sent);

			bool SentGetblocks();

			void SetSentGetblocks(bool sent);

			const std::vector<UInt256> &GetCurrentBlockTxHashes() const;

			void AddCurrentBlockTxHash(const UInt256 &hash);

			const std::vector<UInt256> &GetKnownBlockHashes() const;

			void KnownBlockHashesRemoveRange(size_t index, size_t len);

			void AddKnownBlockHash(const UInt256 &hash);

			const std::vector<UInt256> &GetKnownTxHashes() const;

			void AddKnownTxHash(const UInt256 &hash);

			const UInt256 &GetLastBlockHash() const;

			void SetLastBlockHash(const UInt256 &hash);

//			const TransactionSet &GetKnownTxHashSet() const;

			double getStartTime() const;

			void setStartTime(double time);

			double getPintTime() const;

			void setPingTime(double time);

			void addPongCallbackInfo(const PeerCallbackInfo &callbackInfo);

			void addPongCallback(const PeerCallback &callback);

			PeerCallbackInfo popPongCallbackInfo();

			PeerCallback popPongCallback();

			const std::deque<PeerCallback> &getPongCallbacks() const;

			PeerManager *getPeerManager() const;

			std::string FormatError(int errnum);

			template<typename Arg1, typename... Args>
			inline void Ptrace(const std::string &fmt, const Arg1 &arg1, const Args &... args) {
				std::string peerFmt = "{}:{} ";
				peerFmt += fmt;
				Log::getLogger()->trace(peerFmt.c_str(), getHost(), getPort(), arg1, args...);
			}

			template<typename Arg1, typename... Args>
			inline void Pdebug(const std::string &fmt, const Arg1 &arg1, const Args &... args) {
				std::string peerFmt = "{}:{} ";
				peerFmt += fmt;
				Log::getLogger()->debug(peerFmt.c_str(), getHost(), getPort(), arg1, args...);
			}

			template<typename Arg1, typename... Args>
			inline void Pinfo(const std::string &fmt, const Arg1 &arg1, const Args &... args) {
				std::string peerFmt = "{}:{} ";
				peerFmt += fmt;
				Log::getLogger()->info(peerFmt.c_str(), getHost(), getPort(), arg1, args...);
			}

			template<typename Arg1, typename... Args>
			inline void Pwarn(const std::string &fmt, const Arg1 &arg1, const Args &... args) {
				std::string peerFmt = "{}:{} ";
				peerFmt += fmt;
				Log::getLogger()->warn(peerFmt.c_str(), getHost(), getPort(), arg1, args...);
			}

			template<typename Arg1, typename... Args>
			inline void Perror(const std::string &fmt, const Arg1 &arg1, const Args &... args) {
				std::string peerFmt = "{}:{} ";
				peerFmt += fmt;
				Log::getLogger()->error(peerFmt.c_str(), getHost(), getPort(), arg1, args...);
			}

			template<typename Arg1, typename... Args>
			inline void Pcritical(const std::string &fmt, const Arg1 &arg1, const Args &... args) {
				std::string peerFmt = "{}:{} ";
				peerFmt += fmt;
				Log::getLogger()->critical(peerFmt.c_str(), getHost(), getPort(), arg1, args...);
			}

			template<typename T>
			inline void Ptrace(const T &msg) {
				Log::getLogger()->trace("{}:{} {}", getHost(), getPort(), msg);
			}

			template<typename T>
			inline void Pdebug(const T &msg) {
				Log::getLogger()->trace("{}:{} {}", getHost(), getPort(), msg);
			}

			template<typename T>
			inline void Pinfo(const T &msg) {
				Log::getLogger()->trace("{}:{} {}", getHost(), getPort(), msg);
			}

			template<typename T>
			inline void Pwarn(const T &msg) {
				Log::getLogger()->trace("{}:{} {}", getHost(), getPort(), msg);
			}

			template<typename T>
			inline void Perror(const T &msg) {
				Log::getLogger()->trace("{}:{} {}", getHost(), getPort(), msg);
			}

			template<typename T>
			inline void Pcritical(const T &msg) {
				Log::getLogger()->trace("{}:{} {}", getHost(), getPort(), msg);
			}

		private:
			void initDefaultMessages();

			bool networkIsReachable() const;

			bool isIPv4() const;

			bool acceptMessage(const CMBlock &msg, const std::string &type);

			int openSocket(int domain, double timeout, int *error);

			void peerThreadRoutine();

		private:
			friend class Message;
			PeerInfo _info;

			uint32_t _magicNumber;
			mutable std::string _host;
			ConnectStatus _status;
			int _waitingForNetwork;
			volatile bool _needsFilterUpdate;
			uint64_t _nonce, _feePerKb;
			std::string _useragent;
			uint32_t _version, _lastblock, _earliestKeyTime, _currentBlockHeight;
			double _startTime, _pingTime;
			volatile double _disconnectTime, _mempoolTime;
			bool _sentVerack, _gotVerack, _sentGetaddr, _sentFilter, _sentGetdata, _sentMempool, _sentGetblocks;
			UInt256 _lastBlockHash;
			MerkleBlockPtr _currentBlock;
			std::vector<UInt256> _currentBlockTxHashes, _knownBlockHashes, _knownTxHashes;
			TransactionSet _knownTxHashSet;
			volatile int _socket;

			std::deque<PeerCallbackInfo> _pongInfoList;
			std::deque<PeerCallback> _pongCallbackList;

			typedef boost::shared_ptr<Message> MessagePtr;
			std::map<std::string, MessagePtr> _messages;
			PeerManager *_manager;
			Listener *_listener;
		};

		typedef boost::shared_ptr<Peer> PeerPtr;

	}
}

#endif //__ELASTOS_SDK_PEER_H__
