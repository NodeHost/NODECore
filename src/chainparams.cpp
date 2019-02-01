// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2019 The NodeHost developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0,      uint256("0x00000e7f327d0d50347e5af70c7af54841cf02d8634749f004571c813716a68b"));

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1549058107, // * UNIX timestamp of last checkpoint block
    0,          // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));

static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1740710,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1454124731,
    0,
    100};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x8b;
        pchMessageStart[1] = 0xef;
        pchMessageStart[2] = 0x56;
        pchMessageStart[3] = 0xc9;
        vAlertPubKey = ParseHex("047cfd8b18551cf8ed0811d120663843c55673514282a917205503bf34ca7e9515f9af4cc9830cf9721da5a96c0d9dc17212ffa5b28d7799932dcde06787c9c1cb");
        nDefaultPort = 2155;
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        nSubsidyHalvingInterval = 1050000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // NodeHost: 1 minute
        nTargetSpacing = 2 * 60;  // NodeHost: 2 minutes
        nMaturity = 10;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 25000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 1000;
        nModifierUpdateBlock = 1; // we use the version 2 for NODE
        nStakeMinAge= 8 * 60 * 60; // 8 hours

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * genesis.py -a quark-hash -z "2019 Feb 01 - Panda Exchange Deploys Hybrid Point of Sale and Crypto ATM in Bogota" -t 1549058107 -v 0 -p 04f69a4af08d96e53eb4181c5d67341ceb0c0eb333bed0455b8ac3531a3086b0c59fb6abc0cc4ccfc36862fa7797bb2dd08048da5cc4bfaa3b643943b22553b5fb
         * algorithm: quark-hash
         * merkle hash: cdb34792059ac20c6b2cc99b6a8831054258675d98f00044da937ebce613d740
         * pszTimestamp: 2019 Feb 01 - Panda Exchange Deploys Hybrid Point of Sale and Crypto ATM in Bogota
         * pubkey: 04f69a4af08d96e53eb4181c5d67341ceb0c0eb333bed0455b8ac3531a3086b0c59fb6abc0cc4ccfc36862fa7797bb2dd08048da5cc4bfaa3b643943b22553b5fb
         * time: 1549058107
         * bits: 0x1e0ffff0
         * nonce: 21087891
         * genesis_hash: 00000e7f327d0d50347e5af70c7af54841cf02d8634749f004571c813716a68b
         */
        const char* pszTimestamp = "2019 Feb 01 - Panda Exchange Deploys Hybrid Point of Sale and Crypto ATM in Bogota";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 0 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04f69a4af08d96e53eb4181c5d67341ceb0c0eb333bed0455b8ac3531a3086b0c59fb6abc0cc4ccfc36862fa7797bb2dd08048da5cc4bfaa3b643943b22553b5fb") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1549058107;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 21087891;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000e7f327d0d50347e5af70c7af54841cf02d8634749f004571c813716a68b"));
        assert(genesis.hashMerkleRoot == uint256("0xcdb34792059ac20c6b2cc99b6a8831054258675d98f00044da937ebce613d740"));

        // DNS Seeding
        vSeeds.push_back(CDNSSeedData("seed1.nodehost.online", "seed1.nodehost.online"));
        vSeeds.push_back(CDNSSeedData("seed2.nodehost.online", "seed2.nodehost.online"));
        vSeeds.push_back(CDNSSeedData("seed3.nodehost.online", "seed3.nodehost.online"));

        // NodeHost addresses start with 'N'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 53);
        // NodeHost script addresses start with '3'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 6);
        // NodeHost private keys start with 'K'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 46);
        // NodeHost BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // NodeHost BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        // NodeHost BIP44 coin type is '222' (0x800000de)
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0xde).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "04e5e1e8e4508f2515145545b8f6a08cbcb9e954d7c202e033b6526309b071408e249258e38ccee297e7f1349e15f9e7d2b835dc5121f5a4d8a19171a8645e0237";
        strMasternodePoolDummyAddress = "NZUa2L6iaJ6ovZmLHcv9AjwjzmjUdt7Nh1";
        nStartMasternodePayments = genesis.nTime + 86400; // 24 hours after genesis creation

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x58;
        pchMessageStart[1] = 0xcd;
        pchMessageStart[2] = 0xfc;
        pchMessageStart[3] = 0xa5;
        vAlertPubKey = ParseHex("046351eb67dffb6e2ab772c53b8853370a859319c4d59ceea211831b8ff1c64409377609a0365cb7bffb969a51a90c41514e32d01d07ce52eee92595d752ac5e36");
        nDefaultPort = 12155;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // NodeHost: 1 minute
        nTargetSpacing = 2 * 60;  // NodeHost: 2 minute
        nLastPOWBlock = 200;
        nStakeMinAge= 5 * 60; // 5 minutes
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 1;
        nMaxMoneyOut = 25000000 * COIN;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1549058107;
        genesis.nNonce = 21087891;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000e7f327d0d50347e5af70c7af54841cf02d8634749f004571c813716a68b"));

        vFixedSeeds.clear();
        vSeeds.clear();

        // Testnet NodeHost addresses start with 'n'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 112);
        // Testnet NodeHost script addresses start with '5' or '6'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 12);
        // Testnet private keys start with 'k'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 108);
        // Testnet NodeHost BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Testnet NodeHost BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        // Testnet NodeHost BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "04b6f6c07df34cebf70e6cf4201d9fd9f0e3b7a3bb180426e3a0c600494add34cb469846a896b2ba74f705086d32c6bc022c505117fafcd1bc03ae9843bd59ade9";
        strMasternodePoolDummyAddress = "nJUWzPCtpcg134uqMwiX9FjT8iFikfX68w";
        nStartMasternodePayments = genesis.nTime + 86400; // 24 hours after genesis
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0x75;
        pchMessageStart[1] = 0xf5;
        pchMessageStart[2] = 0x23;
        pchMessageStart[3] = 0xc0;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // NodeHost: 1 day
        nTargetSpacing = 2 * 60;        // NodeHost: 1 minutes
        nStakeMinAge= 5 * 60; // 5 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1549058107;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 20542303;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 22155;
        assert(hashGenesisBlock == uint256("0x6b93303c7307adb52bb4ea2e2e36957b2757861d7b56829119a95a398e1ea779"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
