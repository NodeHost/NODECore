#include "hostingfees.h"
#include "util.h"
#include "main.h"

HostingFees::HostingFees()
{
   address = CBitcoinAddress("n9Jh76BQnRn2A6KpJYtKApPB5kuiPz5MH4");
   address2 = CBitcoinAddress("NZ1TWNhEAL9kcA3AzWcrGZfNAR64uFuKZA");
}

void HostingFees::Create(CMutableTransaction& txNew, const int idxProofOfStake, const int nBlockHeight)
{
    CAmount nReward = GetBlockValue(nBlockHeight);

    int i = txNew.vout.size();
    CAmount nAmount = nReward * fPercentage;

    LogPrint("hostingfees", "HostingFees::Create() vout size: %s, nReward=%d, nAmount=%d\n", i, nReward, nAmount);

    txNew.vout.resize(i + 1);
    txNew.vout[idxProofOfStake].nValue -= nAmount;
    txNew.vout[i].scriptPubKey = GetScriptForDestination(nBlockHeight+1 >= 4500 ? address2.Get() : address.Get());
    txNew.vout[i].nValue = nAmount;
}

bool HostingFees::Validate(const CBlock& block, const int nBlockHeight)
{
    CAmount nReward = GetBlockValue(nBlockHeight);

    int i = block.vtx[1].vout.size();
    CAmount nExpectedAmount = nReward * fPercentage;

    LogPrint("hostingfees", "HostingFees::Validate() vout size: %s, nReward=%d, nExpectedAmount=%d\n", i, nReward, nExpectedAmount);

    if (block.vtx[1].vout[i - 1].scriptPubKey != GetScriptForDestination(address.Get()) &&
        block.vtx[1].vout[i - 1].scriptPubKey != GetScriptForDestination(address2.Get())) {
        LogPrintf("HostingFees::Validate() : ERROR invalid scriptPubKey\n");
        return false;
    }

    if (block.vtx[1].vout[i -1].nValue != nExpectedAmount) {
        LogPrintf("HostingFees::Validate() : ERROR invalid amount nExpectedAmount=%d nValue=%d\n", nExpectedAmount, block.vtx[1].vout[i -1].nValue);
        return false;
    }

    return true;
}