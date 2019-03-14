#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

#include <map>
#include <vector>
#include "./vm/appaccount.h"
#include "serialize.h"
#include "tx.h"
using namespace std;

class CAccount;
class CKeyID;
class uint256;
class CDiskTxPos;
class CVmOperate;

class CAccountView {
   public:
    virtual bool GetAccount(const CKeyID &keyId, CAccount &account);
    virtual bool SetAccount(const CKeyID &keyId, const CAccount &account);
    virtual bool SetAccount(const vector<unsigned char> &accountId, const CAccount &account);
    virtual bool HaveAccount(const CKeyID &keyId);
    virtual uint256 GetBestBlock();
    virtual bool SetBestBlock(const uint256 &hashBlock);
    virtual bool BatchWrite(const map<CKeyID, CAccount> &mapAccounts, const map<vector<unsigned char>, CKeyID> &mapKeyIds, const uint256 &hashBlock);
    virtual bool BatchWrite(const vector<CAccount> &vAccounts);
    virtual bool EraseAccount(const CKeyID &keyId);
    virtual bool SetKeyId(const vector<unsigned char> &accountId, const CKeyID &keyId);
    virtual bool GetKeyId(const vector<unsigned char> &accountId, CKeyID &keyId);
    virtual bool EraseKeyId(const vector<unsigned char> &accountId);
    virtual bool GetAccount(const vector<unsigned char> &accountId, CAccount &account);
    virtual bool SaveAccountInfo(const vector<unsigned char> &accountId, const CKeyID &keyId, const CAccount &account);
    virtual std::tuple<uint64_t, uint64_t> TraverseAccount() = 0;
    virtual Object ToJsonObj(char Prefix);
    virtual ~CAccountView(){};
};

class CAccountViewBacked : public CAccountView {
   protected:
    CAccountView *pBase;

   public:
    CAccountViewBacked(CAccountView &accountView);

    bool GetAccount(const CKeyID &keyId, CAccount &account);
    bool SetAccount(const CKeyID &keyId, const CAccount &account);
    bool SetAccount(const vector<unsigned char> &accountId, const CAccount &account);
    bool HaveAccount(const CKeyID &keyId);
    uint256 GetBestBlock();
    bool SetBestBlock(const uint256 &hashBlock);
    bool BatchWrite(const map<CKeyID, CAccount> &mapAccounts, const map<vector<unsigned char>, CKeyID> &mapKeyIds, const uint256 &hashBlock);
    bool BatchWrite(const vector<CAccount> &vAccounts);
    bool EraseAccount(const CKeyID &keyId);
    bool SetKeyId(const vector<unsigned char> &accountId, const CKeyID &keyId);
    bool GetKeyId(const vector<unsigned char> &accountId, CKeyID &keyId);
    bool EraseKeyId(const vector<unsigned char> &accountId);
    bool GetAccount(const vector<unsigned char> &accountId, CAccount &account);
    bool SaveAccountInfo(const vector<unsigned char> &accountId, const CKeyID &keyId, const CAccount &account);
    std::tuple<uint64_t, uint64_t> TraverseAccount();
};

class CAccountViewCache : public CAccountViewBacked {
   public:
    uint256 hashBlock;
    map<CKeyID, CAccount> cacheAccounts;
    map<vector<unsigned char>, CKeyID> cacheKeyIds;  // vector of account KeyIds

   private:
    bool GetAccount(const CKeyID &keyId, CAccount &account);
    bool SetAccount(const CKeyID &keyId, const CAccount &account);
    bool SetAccount(const vector<unsigned char> &accountId, const CAccount &account);
    bool HaveAccount(const CKeyID &keyId);
    bool EraseAccount(const CKeyID &keyId);
    bool SetKeyId(const vector<unsigned char> &accountId, const CKeyID &keyId);
    bool GetKeyId(const vector<unsigned char> &accountId, CKeyID &keyId);
    bool EraseKeyId(const vector<unsigned char> &accountId);
    bool GetAccount(const vector<unsigned char> &accountId, CAccount &account);

   public:
    CAccountViewCache(CAccountView &base, bool fDummy = false);

    uint256 GetBestBlock();
    bool SetBestBlock(const uint256 &hashBlock);
    bool BatchWrite(const map<CKeyID, CAccount> &mapAccounts, const map<vector<unsigned char>, CKeyID> &mapKeyIds, const uint256 &hashBlock);
    bool BatchWrite(const vector<CAccount> &vAccounts);
    /**
     * @brief from use id to reg id
     * @param userId
     * @param regId
     * @return
     */

    bool GetAccount(const CUserID &userId, CAccount &account);
    bool SetAccount(const CUserID &userId, const CAccount &account);
    bool GetKeyId(const CUserID &userId, CKeyID &keyId);
    bool SetKeyId(const CUserID &userId, const CKeyID &keyId);
    bool GetUserId(const string &addr, CUserID &userId);
    bool GetRegId(const CKeyID &keyId, CRegID &regId);
    bool GetRegId(const CUserID &userId, CRegID &regId) const;
    bool EraseAccount(const CUserID &userId);
    bool EraseId(const CUserID &userId);
    bool HaveAccount(const CUserID &userId);
    int64_t GetRawBalance(const CUserID &userId) const;
    bool SaveAccountInfo(const CRegID &accountId, const CKeyID &keyId, const CAccount &account);
    bool Flush();
    unsigned int GetCacheSize();
    Object ToJsonObj() const;
    void SetBaseData(CAccountView *pBase);
};

class CScriptDBView {
   public:
    virtual bool GetData(const vector<unsigned char> &vKey, vector<unsigned char> &vValue);
    virtual bool SetData(const vector<unsigned char> &vKey, const vector<unsigned char> &vValue);
    virtual bool BatchWrite(const map<vector<unsigned char>, vector<unsigned char> > &mapContractDb);
    virtual bool EraseKey(const vector<unsigned char> &vKey);
    virtual bool HasData(const vector<unsigned char> &vKey);
    virtual bool GetScript(const int &nIndex, vector<unsigned char> &vScriptId, vector<unsigned char> &vValue);
    virtual bool GetContractData(const int nCurBlockHeight, const vector<unsigned char> &vScriptId, const int &nIndex,
                                 vector<unsigned char> &vScriptKey, vector<unsigned char> &vScriptData);
    virtual Object ToJsonObj(string Prefix);
    virtual bool ReadTxIndex(const uint256 &txid, CDiskTxPos &pos);
    virtual bool WriteTxIndex(const vector<pair<uint256, CDiskTxPos> > &list, vector<CScriptDBOperLog> &vTxIndexOperDB);
    virtual bool WriteTxOutPut(const uint256 &txid, const vector<CVmOperate> &vOutput, CScriptDBOperLog &operLog);
    virtual bool ReadTxOutPut(const uint256 &txid, vector<CVmOperate> &vOutput);
    virtual bool GetTxHashByAddress(const CKeyID &keyId, int nHeight, map<vector<unsigned char>, vector<unsigned char> > &mapTxHash);
    virtual bool SetTxHashByAddress(const CKeyID &keyId, int nHeight, int nIndex, const string &strTxHash, CScriptDBOperLog &operLog);
    virtual bool GetAllScriptAcc(const CRegID &scriptId, map<vector<unsigned char>, vector<unsigned char> > &mapAcc);
    virtual ~CScriptDBView(){};
};

class CScriptDBViewBacked : public CScriptDBView {
   protected:
    CScriptDBView *pBase;

   public:
    CScriptDBViewBacked(CScriptDBView &dataBaseView);
    bool GetData(const vector<unsigned char> &vKey, vector<unsigned char> &vValue);
    bool SetData(const vector<unsigned char> &vKey, const vector<unsigned char> &vValue);
    bool BatchWrite(const map<vector<unsigned char>, vector<unsigned char> > &mapContractDb);
    bool EraseKey(const vector<unsigned char> &vKey);
    bool HasData(const vector<unsigned char> &vKey);
    bool GetScript(const int &nIndex, vector<unsigned char> &vScriptId, vector<unsigned char> &vValue);
    bool GetContractData(const int nCurBlockHeight, const vector<unsigned char> &vScriptId, const int &nIndex,
                         vector<unsigned char> &vScriptKey, vector<unsigned char> &vScriptData);
    bool ReadTxIndex(const uint256 &txid, CDiskTxPos &pos);
    bool WriteTxIndex(const vector<pair<uint256, CDiskTxPos> > &list, vector<CScriptDBOperLog> &vTxIndexOperDB);
    bool WriteTxOutPut(const uint256 &txid, const vector<CVmOperate> &vOutput, CScriptDBOperLog &operLog);
    bool ReadTxOutPut(const uint256 &txid, vector<CVmOperate> &vOutput);
    bool GetTxHashByAddress(const CKeyID &keyId, int nHeight, map<vector<unsigned char>, vector<unsigned char> > &mapTxHash);
    bool SetTxHashByAddress(const CKeyID &keyId, int nHeight, int nIndex, const string &strTxHash, CScriptDBOperLog &operLog);
    bool GetAllScriptAcc(const CRegID &scriptId, map<vector<unsigned char>, vector<unsigned char> > &mapAcc);
};

class CScriptDBViewCache : public CScriptDBViewBacked {
   public:
    map<vector<unsigned char>, vector<unsigned char> > mapContractDb;
    /*取脚本 时 第一个vector 是scriptKey = "def" + "scriptid";
      取应用账户时第一个vector是scriptKey = "acct" + "scriptid"+"_" + "accUserId";
      取脚本总条数时第一个vector是scriptKey ="snum",
      取脚本数据总条数时第一个vector是scriptKey ="sdnum";
      取脚本数据时第一个vector是scriptKey ="data" + "vScriptId" + "_" + "vScriptKey"
      取交易关联账户时第一个vector是scriptKey ="tx" + "txHash"
    */
   public:
    CScriptDBViewCache(CScriptDBView &base, bool fDummy = false);
    bool GetScript(const CRegID &scriptId, vector<unsigned char> &vValue);
    bool GetScriptAcc(const CRegID &scriptId, const vector<unsigned char> &vKey, CAppUserAccount &appAccOut);
    bool SetScriptAcc(const CRegID &scriptId, const CAppUserAccount &appAccIn, CScriptDBOperLog &operlog);
    bool EraseScriptAcc(const CRegID &scriptId, const vector<unsigned char> &vKey);
    bool GetScript(const int nIndex, CRegID &scriptId, vector<unsigned char> &vValue);
    bool SetScript(const CRegID &scriptId, const vector<unsigned char> &vValue);
    bool HaveScript(const CRegID &scriptId);
    bool EraseScript(const CRegID &scriptId);
    bool GetContractItemCount(const CRegID &scriptId, int &nCount);
    bool EraseAppData(const CRegID &scriptId, const vector<unsigned char> &vScriptKey, CScriptDBOperLog &operLog);
    bool HasScriptData(const CRegID &scriptId, const vector<unsigned char> &vScriptKey);
    bool GetContractData(const int nCurBlockHeight, const CRegID &scriptId, const vector<unsigned char> &vScriptKey,
                         vector<unsigned char> &vScriptData);
    bool GetContractData(const int nCurBlockHeight, const CRegID &scriptId, const int &nIndex,
                         vector<unsigned char> &vScriptKey, vector<unsigned char> &vScriptData);
    bool SetContractData(const CRegID &scriptId, const vector<unsigned char> &vScriptKey,
                         const vector<unsigned char> &vScriptData, CScriptDBOperLog &operLog);
    bool SetDelegateData(const CAccount &delegateAcct, CScriptDBOperLog &operLog);
    bool SetDelegateData(const vector<unsigned char> &vKey);
    bool EraseDelegateData(const CAccount &delegateAcct, CScriptDBOperLog &operLog);
    bool EraseDelegateData(const vector<unsigned char> &vKey);
    bool UndoScriptData(const vector<unsigned char> &vKey, const vector<unsigned char> &vValue);
    /**
     * @brief Get all number of scripts in scriptdb
     * @param nCount
     * @return true if get succeed, otherwise false
     */
    bool GetScriptCount(int &nCount);
    bool SetTxRelAccout(const uint256 &txHash, const set<CKeyID> &relAccount);
    bool GetTxRelAccount(const uint256 &txHash, set<CKeyID> &relAccount);
    bool EraseTxRelAccout(const uint256 &txHash);
    /**
     * @brief write all data in the caches to script db
     * @return
     */
    bool Flush();
    unsigned int GetCacheSize();
    Object ToJsonObj() const;
	CScriptDBView * GetBaseScriptDB() { return pBase; }
    bool ReadTxIndex(const uint256 &txid, CDiskTxPos &pos);
    bool WriteTxIndex(const vector<pair<uint256, CDiskTxPos> > &list, vector<CScriptDBOperLog> &vTxIndexOperDB);
    void SetBaseData(CScriptDBView *pBase);
    string ToString();
    bool WriteTxOutPut(const uint256 &txid, const vector<CVmOperate> &vOutput, CScriptDBOperLog &operLog);
    bool ReadTxOutPut(const uint256 &txid, vector<CVmOperate> &vOutput);
    bool GetTxHashByAddress(const CKeyID &keyId, int nHeight, map<vector<unsigned char>, vector<unsigned char> > &mapTxHash);
    bool SetTxHashByAddress(const CKeyID &keyId, int nHeight, int nIndex, const string &strTxHash, CScriptDBOperLog &operLog);
    bool GetAllScriptAcc(const CRegID &scriptId, map<vector<unsigned char>, vector<unsigned char> > &mapAcc);

   private:
    bool GetData(const vector<unsigned char> &vKey, vector<unsigned char> &vValue);
    bool SetData(const vector<unsigned char> &vKey, const vector<unsigned char> &vValue);
    bool BatchWrite(const map<vector<unsigned char>, vector<unsigned char> > &mapContractDb);
    bool EraseKey(const vector<unsigned char> &vKey);
    bool HasData(const vector<unsigned char> &vKey);

    /**
     * @brief Get script content from scriptdb by scriptid
     * @param vScriptId
     * @param vValue
     * @return true if get script succeed,otherwise false
     */
    bool GetScript(const vector<unsigned char> &vScriptId, vector<unsigned char> &vValue);
    /**
     * @brief Get Script content from scriptdb by index
     * @param nIndex the value must be non-negative
     * @param vScriptId
     * @param vValue
     * @return true if get script succeed, otherwise false
     */
    bool GetScript(const int nIndex, vector<unsigned char> &vScriptId, vector<unsigned char> &vValue);
    /**
     * @brief Save script content to scriptdb
     * @param vScriptId
     * @param vValue
     * @return true if save succeed, otherwise false
     */
    bool SetScript(const vector<unsigned char> &vScriptId, const vector<unsigned char> &vValue);
    /**
     * @brief Detect if scriptdb contains the script by scriptid
     * @param vScriptId
     * @return true if contains script, otherwise false
     */
    bool HaveScript(const vector<unsigned char> &vScriptId);
    /**
     * @brief Save all number of scripts in scriptdb
     * @param nCount
     * @return true if save count succeed, otherwise false
     */
    bool SetScriptCount(const int nCount);
    /**
     * @brief Delete script from script db by scriptId
     * @param vScriptId
     * @return true if delete succeed, otherwise false
     */
    bool EraseScript(const vector<unsigned char> &vScriptId);
    /**
     * @brief Get total number of contract data elements in contract db
     * @param vScriptId
     * @param nCount
     * @return true if get succeed, otherwise false
     */
    bool GetContractItemCount(const vector<unsigned char> &vScriptId, int &nCount);
    /**
     * @brief Save count of the Contract's data into contract db
     * @param vScriptId
     * @param nCount
     * @return true if save succeed, otherwise false
     */
    bool SetContractItemCount(const vector<unsigned char> &vScriptId, int nCount);
    /**
     * @brief Delete the item of the scirpt's data by scriptId and scriptKey
     * @param vScriptId
     * @param vScriptKey must be 8 bytes
     * @return true if delete succeed, otherwise false
     */
    bool EraseAppData(const vector<unsigned char> &vScriptId, const vector<unsigned char> &vScriptKey, CScriptDBOperLog &operLog);

    bool EraseAppData(const vector<unsigned char> &vKey);
    /**
     * @brief Detect if scriptdb contains the item of script's data by scriptid and scriptkey
     * @param vScriptId
     * @param vScriptKey must be 8 bytes
     * @return true if contains the item, otherwise false
     */
    bool HasScriptData(const vector<unsigned char> &vScriptId, const vector<unsigned char> &vScriptKey);
    /**
     * @brief Get smart contract App data and valid height by scriptid and scriptkey
     * @param vScriptId
     * @param vScriptKey must be 8 bytes
     * @param vScriptData
     * @param nHeight valide height of script data
     * @return true if get succeed, otherwise false
     */
    bool GetContractData(const int nCurBlockHeight, const vector<unsigned char> &vScriptId, const vector<unsigned char> &vScriptKey,
                         vector<unsigned char> &vScriptData);
    /**
     * @brief Get smart contract app data and valid height by scriptid and nIndex
     * @param vScriptId
     * @param nIndex get first script data will be 0, otherwise be 1
     * @param vScriptKey must be 8 bytes, get first script data will be empty, otherwise get next scirpt data will be previous script key
     * @param vScriptData
     * @param nHeight valid height of script data
     * @return true if get succeed, otherwise false
     */
    bool GetContractData(const int nCurBlockHeight, const vector<unsigned char> &vScriptId, const int &nIndex, vector<unsigned char> &vScriptKey, vector<unsigned char> &vScriptData);
    /**
     * @brief Save script data and valid height into script db
     * @param vScriptId
     * @param vScriptKey must be 8 bytes
     * @param vScriptData
     * @param nHeight valide height of script data
     * @return true if save succeed, otherwise false
     */
    bool SetContractData(const vector<unsigned char> &vScriptId, const vector<unsigned char> &vScriptKey,
                         const vector<unsigned char> &vScriptData, CScriptDBOperLog &operLog);
};

class CTransactionDBView {
   public:
    virtual uint256 HasTx(const uint256 &txHash);
    virtual bool IsContainBlock(const CBlock &block);
    virtual bool AddBlockToCache(const CBlock &block);
    virtual bool DeleteBlockFromCache(const CBlock &block);
    virtual bool LoadTransaction(map<uint256, vector<uint256> > &mapTxHashByBlockHash);
    virtual bool BatchWrite(const map<uint256, vector<uint256> > &mapTxHashByBlockHashIn);
    virtual ~CTransactionDBView(){};
};

class CTransactionDBViewBacked : public CTransactionDBView {
   protected:
    CTransactionDBView *pBase;
    bool LoadTransaction(map<uint256, vector<uint256> > &mapTxHashByBlockHash);

   public:
    CTransactionDBViewBacked(CTransactionDBView &transactionView);
    bool BatchWrite(const map<uint256, vector<uint256> > &mapTxHashByBlockHashIn);
    uint256 HasTx(const uint256 &txHash);
    bool IsContainBlock(const CBlock &block);
    bool AddBlockToCache(const CBlock &block);
    bool DeleteBlockFromCache(const CBlock &block);
};

class CTransactionDBCache : public CTransactionDBViewBacked {
   private:
    CTransactionDBCache(CTransactionDBCache &transactionView);
    map<uint256, vector<uint256> > mapTxHashByBlockHash;  // key:block hash  value:tx hash
    bool IsInMap(const map<uint256, vector<uint256> > &mMap, const uint256 &hash) const;

   public:
    CTransactionDBCache(CTransactionDBView &pTxCacheDB, bool fDummy);
    bool IsContainBlock(const CBlock &block);
    bool AddBlockToCache(const CBlock &block);
    bool DeleteBlockFromCache(const CBlock &block);
    uint256 HasTx(const uint256 &txHash);
    map<uint256, vector<uint256> > GetTxHashCache(void);
    bool BatchWrite(const map<uint256, vector<uint256> > &mapTxHashByBlockHashIn);
    void AddTxHashCache(const uint256 &blockHash, const vector<uint256> &vTxHash);
    bool Flush();
    bool LoadTransaction();
    void Clear();
    Object ToJsonObj() const;
    int GetSize();
    void SetBaseData(CTransactionDBView *pNewBase);
    const map<uint256, vector<uint256> > &GetCacheMap();
    void SetCacheMap(const map<uint256, vector<uint256> > &mapCache);
};

//class CDelegateDBView {
//
//public:
//    virtual bool ModifyDelegatesVote(const vector<unsigned char> &vScriptId, uint64_t llVotes);
//    virtual bool BatchWrite(const map<vector<unsigned char>, bool> &mapVoteAccount);
//    virtual bool GetTopXDelegateAccount(vector<vector<unsigned char> > &listScriptId);
//    virtual ~CDelegateDBView(){};
//};

//class CDelegateDBViewBacked : public CDelegateDBView {
//protected:
//    CDelegateDBView * pBase;
//public:
//    bool ModifyDelegatesVote(const vector<unsigned char> &vScriptId, uint64_t llVotes);
//    bool BatchWrite(const map< vector<unsigned char>, bool> &mapVoteAccount);
//    bool GetTopXDelegateAccount(int n, vector<vector<unsigned char> > &listScriptId);
//};
//
//
//class CDelegateDBCache : public CDelegateDBViewBacked{
//public:
//    map<vector<unsigned char>, bool> ;
//private:
//    CDelegateDBCache(CDelegateDBCache &DelegateView);
//public:
//    bool ModifyDelegatesVote(const vector<unsigned char> &vScriptId, uint64_t llVotes);
//    bool BatchWrite(const map< vector<unsigned char>, bool> &mapVoteAccount);
//    bool GetTopXDelegateAccount(int n, vector<vector<unsigned char> > &listScriptId);
//};

#endif
