#ifndef HOSTINGFEES_H
#define HOSTINGFEES_H

#include "base58.h"

class HostingFees
{
public:
    HostingFees();
    void Create(CMutableTransaction& txNew, const int idxProofOfStake, const int nBlockHeight);
    bool Validate(const CBlock& block, const int nBlockHeight);
private:
    CBitcoinAddress address;
    const float fPercentage{0.02};
};

#endif // HOSTINGFEES_H
