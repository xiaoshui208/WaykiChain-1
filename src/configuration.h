/*
 * configuration.h
 *
 *  Created on: 2016年9月8日
 *      Author: WaykiChain Core Developer
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <memory>
#include "bignum.h"
#include "uint256.h"
#include "arith_uint256.h"
#include "util.h"
#include "chainparams.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <vector>
#include <map>

using namespace std;

class CBlockIndex;
class uint256;

/** Block-chain checkpoints are compiled-in sanity checks.
 * They are updated every release or three.
 */
namespace Checkpoints
{
    // Returns true if block passes checkpoint checks
    bool CheckBlock(int nHeight, const uint256& hash);

    // Return conservative estimate of total number of blocks, 0 if unknown
    int GetTotalBlocksEstimate();

    // Returns last CBlockIndex* in mapBlockIndex that is a checkpoint
    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex);

    double GuessVerificationProgress(CBlockIndex *pindex, bool fSigchecks = true);

    bool AddCheckpoint(int nHeight, uint256 hash);

    bool GetCheckpointByHeight(const int nHeight, std::vector<int> &vCheckpoints);

    bool LoadCheckpoint();

    void GetCheckpointMap(std::map<int, uint256> &checkpoints);

    extern bool fEnabled;
}

typedef enum  {
	emNODE_1,
	emNODE_2,
	MAX_NODE		//!< MAX_NETWORK_TYPES
} NODE_NUMBER;

class G_CONFIG_TABLE
{
public:
	string GetCoinName()const
	{
		return /*std::move(COIN_NAME)*/COIN_NAME;
	}

	const vector<string> GetIntPubKey(NET_TYPE type) const;
	const uint256 GetIntHash(NET_TYPE type) const;
	const string GetCheckPointPkey(NET_TYPE type) const;
	const uint256 GetHashMerkleRoot() const;
	vector<unsigned int> GetSeedNodeIP() const;
	unsigned char* GetMagicNumber(NET_TYPE type) const;
	vector<unsigned char> GetAddressPrefix(NET_TYPE type,Base58Type BaseType) const;
	unsigned int GetnDefaultPort(NET_TYPE type) const;
	unsigned int GetnRPCPort(NET_TYPE type) const;
	unsigned int GetnUIPort(NET_TYPE type) const;
	unsigned int GetStartTimeInit(NET_TYPE type) const;
	unsigned int GetHalvingInterval(NET_TYPE type) const;
	uint64_t GetCoinInitValue() const;
	uint64_t GetBlockSubsidyCfg(int nHeight) const;
	int GetBlockSubsidyJumpHeight(uint64_t nSubsidyValue) const;
	uint64_t GetDelegatesNum() const;
	uint32_t GetExpansionPecentCfg() const;
	string GetDelegateSignature(NET_TYPE type) const;
	const vector<string> GetDelegatePubKey(NET_TYPE type) const;

private:
	static string COIN_NAME ;	/* basecoin name */

	/* initial public key */
	static  vector<string> intPubKey_mainNet;
	static  vector<string> initPubKey_testNet;
	static  vector<string> initPubkey_regTest;

	/* delegate public key */
	static vector<string> delegatePubKey_mainNet;
	static vector<string> delegatePubKey_testNet;
	static vector<string> delegatePubKey_regTest;

	/* delegate signature */
	static string delegateSignature_mainNet;
	static string delegateSignature_testNet;
    static string delegateSignature_regNet;

	/* gensis block hash */
	static string hashGenesisBlock_mainNet;
	static string hashGenesisBlock_testNet;
	static string hashGenesisBlock_regTest;

	/* checkpoint public key */
	static string CheckPointPK_MainNet;
	static string CheckPointPK_TestNet;

	/* merkle root hash */
	static string HashMerkleRoot;

	/* Peer IP seeds */
	static vector<unsigned int> pnSeed;

	/* Network Magic Number */
	static unsigned char Message_mainNet[MESSAGE_START_SIZE];
	static unsigned char Message_testNet[MESSAGE_START_SIZE];
	static unsigned char Message_regTest[MESSAGE_START_SIZE];

	/* Address Prefix */
	static  vector<unsigned char> AddrPrefix_mainNet[MAX_BASE58_TYPES];
	static  vector<unsigned char> AddrPrefix_testNet[MAX_BASE58_TYPES];

	/* P2P Port */
	static unsigned int nDefaultPort_mainNet ;
	static unsigned int nDefaultPort_testNet ;
	static unsigned int nDefaultPort_regTest;

	/* RPC Port */
	static unsigned int nRPCPort_mainNet;
	static unsigned int nRPCPort_testNet ;

	/* UI Port */
	static unsigned int nUIPort_mainNet;
	static unsigned int nUIPort_testNet;

	/* Start Time */
	static unsigned int StartTime_mainNet;
	static unsigned int StartTime_testNet;
	static unsigned int StartTime_regTest;

	/* Subsidy Halving Interval (半衰期) */
	static unsigned int nSubsidyHalvingInterval_mainNet;
	static unsigned int nSubsidyHalvingInterval_testNet;
	static unsigned int nSubsidyHalvingInterval_regNet;

	/* initial coin */
	static uint64_t InitialCoin;

	/* Default Miner fee */
	static uint64_t DefaultFee;

	/* Designated Delegate count */
	static unsigned int nDelegates;

	static unsigned int ExpansionPecent;

	/* Initial subsidy rate upon vote casting */
	static uint64_t nInitialSubsidy;
	/* Eventual/lasting subsidy rate for vote casting */
	static uint64_t nFixedSubsidy;

};

const G_CONFIG_TABLE &IniCfg();



#endif /* CONFIGURATION_H_ */
