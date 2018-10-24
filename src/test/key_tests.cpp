// Copyright (c) 2012-2013 The Bitcoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "key.h"

#include "base58.h"
#include "script/script.h"
#include "uint256.h"
#include "util.h"
#include "utilstrencodings.h"

#include <string>
#include <vector>

#include <boost/test/unit_test.hpp>

using namespace std;

static const string strSecret1     ("2aDZkc9uvejYHpMf1kLzxzQenPN8ZBkWBPu9UDP3WaDWdDGiNEQ");
static const string strSecret2     ("2ZXKDGFKZuUtjUEVBkPLbrEgYYLToco28sYkiuHevSqJNYiGSof");
static const string strSecret1C    ("7wcTZUALboYm7E2AnJKGr8PPmKiGM4T1nt5AP7eJ5XcDkVB5AYrE");
static const string strSecret2C    ("7tYoP8JCxEe7LTCXQSVbWpKPYDJuJkz81k79ywnWNu2BfjxCeDB8");
static const CBitcoinAddress addr1 ("NRvZjYVt3UqeJ9LmvyntvjvFETKvwiY21S");
static const CBitcoinAddress addr2 ("NQ4KcmsD8eAVHbc4addKMhy14tBEuw5wMJ");
static const CBitcoinAddress addr1C("NU44snkVSazhvr1mgN9JVDm1W9pPYCMmjQ");
static const CBitcoinAddress addr2C("NYcnJvtAmaoWoxEZrkBN9rdjuW9dLqFQRh");


static const string strAddressBad("Xta1praZQjyELweyMByXyiREw1ZRsjXzVP");

#ifdef KEY_TESTS_DUMPINFO
void dumpKeyInfo()
{
    CKey k;
    k.MakeNewKey(false);
    CPrivKey s = k.GetPrivKey();

    printf("Generating new key\n");

    for (int nCompressed=0; nCompressed<2; nCompressed++)
    {
        bool bCompressed = nCompressed == 1;

        CKey key;
        key.SetPrivKey(s, bCompressed);
        CPrivKey secret = key.GetPrivKey();
        CPubKey pubKey = key.GetPubKey();

        CBitcoinSecret bsecret;
        bsecret.SetKey(key);

        printf("  * %s:\n", bCompressed ? "compressed" : "uncompressed");
        printf("    * secret (base58): %s\n", bsecret.ToString().c_str());
        printf("    * pubkey (hex): %s\n", HexStr(pubKey).c_str());
        printf("    * address (base58): %s\n", CBitcoinAddress(CTxDestination(pubKey.GetID())).ToString().c_str());
}
#endif


BOOST_AUTO_TEST_SUITE(key_tests)

BOOST_AUTO_TEST_CASE(key_test1)
{
    CBitcoinSecret bsecret1, bsecret2, bsecret1C, bsecret2C, baddress1;
    BOOST_CHECK( bsecret1.SetString (strSecret1));
    BOOST_CHECK( bsecret2.SetString (strSecret2));
    BOOST_CHECK( bsecret1C.SetString(strSecret1C));
    BOOST_CHECK( bsecret2C.SetString(strSecret2C));
    BOOST_CHECK(!baddress1.SetString(strAddressBad));

    CKey key1  = bsecret1.GetKey();
    BOOST_CHECK(key1.IsCompressed() == false);
    CKey key2  = bsecret2.GetKey();
    BOOST_CHECK(key2.IsCompressed() == false);
    CKey key1C = bsecret1C.GetKey();
    BOOST_CHECK(key1C.IsCompressed() == true);
    CKey key2C = bsecret2C.GetKey();
    BOOST_CHECK(key2C.IsCompressed() == true);

    CPubKey pubkey1  = key1. GetPubKey();
    CPubKey pubkey2  = key2. GetPubKey();
    CPubKey pubkey1C = key1C.GetPubKey();
    CPubKey pubkey2C = key2C.GetPubKey();

    BOOST_CHECK(key1.VerifyPubKey(pubkey1));
    BOOST_CHECK(!key1.VerifyPubKey(pubkey1C));
    BOOST_CHECK(!key1.VerifyPubKey(pubkey2));
    BOOST_CHECK(!key1.VerifyPubKey(pubkey2C));

    BOOST_CHECK(!key1C.VerifyPubKey(pubkey1));
    BOOST_CHECK(key1C.VerifyPubKey(pubkey1C));
    BOOST_CHECK(!key1C.VerifyPubKey(pubkey2));
    BOOST_CHECK(!key1C.VerifyPubKey(pubkey2C));

    BOOST_CHECK(!key2.VerifyPubKey(pubkey1));
    BOOST_CHECK(!key2.VerifyPubKey(pubkey1C));
    BOOST_CHECK(key2.VerifyPubKey(pubkey2));
    BOOST_CHECK(!key2.VerifyPubKey(pubkey2C));

    BOOST_CHECK(!key2C.VerifyPubKey(pubkey1));
    BOOST_CHECK(!key2C.VerifyPubKey(pubkey1C));
    BOOST_CHECK(!key2C.VerifyPubKey(pubkey2));
    BOOST_CHECK(key2C.VerifyPubKey(pubkey2C));

    BOOST_CHECK(addr1.Get()  == CTxDestination(pubkey1.GetID()));
    BOOST_CHECK(addr2.Get()  == CTxDestination(pubkey2.GetID()));
    BOOST_CHECK(addr1C.Get() == CTxDestination(pubkey1C.GetID()));
    BOOST_CHECK(addr2C.Get() == CTxDestination(pubkey2C.GetID()));

    for (int n=0; n<16; n++)
    {
        string strMsg = strprintf("Very secret message %i: 11", n);
        uint256 hashMsg = Hash(strMsg.begin(), strMsg.end());

        // normal signatures

        vector<unsigned char> sign1, sign2, sign1C, sign2C;

        BOOST_CHECK(key1.Sign (hashMsg, sign1));
        BOOST_CHECK(key2.Sign (hashMsg, sign2));
        BOOST_CHECK(key1C.Sign(hashMsg, sign1C));
        BOOST_CHECK(key2C.Sign(hashMsg, sign2C));

        BOOST_CHECK( pubkey1.Verify(hashMsg, sign1));
        BOOST_CHECK(!pubkey1.Verify(hashMsg, sign2));
        BOOST_CHECK( pubkey1.Verify(hashMsg, sign1C));
        BOOST_CHECK(!pubkey1.Verify(hashMsg, sign2C));

        BOOST_CHECK(!pubkey2.Verify(hashMsg, sign1));
        BOOST_CHECK( pubkey2.Verify(hashMsg, sign2));
        BOOST_CHECK(!pubkey2.Verify(hashMsg, sign1C));
        BOOST_CHECK( pubkey2.Verify(hashMsg, sign2C));

        BOOST_CHECK( pubkey1C.Verify(hashMsg, sign1));
        BOOST_CHECK(!pubkey1C.Verify(hashMsg, sign2));
        BOOST_CHECK( pubkey1C.Verify(hashMsg, sign1C));
        BOOST_CHECK(!pubkey1C.Verify(hashMsg, sign2C));

        BOOST_CHECK(!pubkey2C.Verify(hashMsg, sign1));
        BOOST_CHECK( pubkey2C.Verify(hashMsg, sign2));
        BOOST_CHECK(!pubkey2C.Verify(hashMsg, sign1C));
        BOOST_CHECK( pubkey2C.Verify(hashMsg, sign2C));

        // compact signatures (with key recovery)

        vector<unsigned char> csign1, csign2, csign1C, csign2C;

        BOOST_CHECK(key1.SignCompact (hashMsg, csign1));
        BOOST_CHECK(key2.SignCompact (hashMsg, csign2));
        BOOST_CHECK(key1C.SignCompact(hashMsg, csign1C));
        BOOST_CHECK(key2C.SignCompact(hashMsg, csign2C));

        CPubKey rkey1, rkey2, rkey1C, rkey2C;

        BOOST_CHECK(rkey1.RecoverCompact (hashMsg, csign1));
        BOOST_CHECK(rkey2.RecoverCompact (hashMsg, csign2));
        BOOST_CHECK(rkey1C.RecoverCompact(hashMsg, csign1C));
        BOOST_CHECK(rkey2C.RecoverCompact(hashMsg, csign2C));

        BOOST_CHECK(rkey1  == pubkey1);
        BOOST_CHECK(rkey2  == pubkey2);
        BOOST_CHECK(rkey1C == pubkey1C);
        BOOST_CHECK(rkey2C == pubkey2C);
    }

    // test deterministic signing

    std::vector<unsigned char> detsig, detsigc;
    string strMsg = "Very deterministic message";
    uint256 hashMsg = Hash(strMsg.begin(), strMsg.end());
    BOOST_CHECK(key1.Sign(hashMsg, detsig));
    BOOST_CHECK(key1C.Sign(hashMsg, detsigc));
    BOOST_CHECK(detsig == detsigc);
    BOOST_CHECK(detsig == ParseHex("304402203a6d91b3e5e4e2996a7bffbf7699aeba634d26c641cf46d126bfb7106908825c022045fe92fdb855dd6ffa2e1b9f3ac36cd75349d5f23590d80761dbcca2a6394ac2"));
    BOOST_CHECK(key2.Sign(hashMsg, detsig));
    BOOST_CHECK(key2C.Sign(hashMsg, detsigc));
    BOOST_CHECK(detsig == detsigc);
    BOOST_CHECK(detsig == ParseHex("3045022100e028b871212be773b3a7638ddf5a38988278d32324da21ef13087830edc3d28b02201c19631ff861f8cb6adb4d73f8a47453dcd260e4737e4f749ec83de47e896ec2"));
    BOOST_CHECK(key1.SignCompact(hashMsg, detsig));
    BOOST_CHECK(key1C.SignCompact(hashMsg, detsigc));
    BOOST_CHECK(detsig == ParseHex("1b3a6d91b3e5e4e2996a7bffbf7699aeba634d26c641cf46d126bfb7106908825c45fe92fdb855dd6ffa2e1b9f3ac36cd75349d5f23590d80761dbcca2a6394ac2"));
    BOOST_CHECK(detsigc == ParseHex("1f3a6d91b3e5e4e2996a7bffbf7699aeba634d26c641cf46d126bfb7106908825c45fe92fdb855dd6ffa2e1b9f3ac36cd75349d5f23590d80761dbcca2a6394ac2"));
    BOOST_CHECK(key2.SignCompact(hashMsg, detsig));
    BOOST_CHECK(key2C.SignCompact(hashMsg, detsigc));
    BOOST_CHECK(detsig == ParseHex("1ce028b871212be773b3a7638ddf5a38988278d32324da21ef13087830edc3d28b1c19631ff861f8cb6adb4d73f8a47453dcd260e4737e4f749ec83de47e896ec2"));
    BOOST_CHECK(detsigc == ParseHex("20e028b871212be773b3a7638ddf5a38988278d32324da21ef13087830edc3d28b1c19631ff861f8cb6adb4d73f8a47453dcd260e4737e4f749ec83de47e896ec2"));
}

BOOST_AUTO_TEST_SUITE_END()
