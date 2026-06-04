#include "global.h"

// tomcrypt_cfg.h redefines malloc, realloc, calloc
#pragma warning(push)
#pragma warning(disable : 4565)
#include <tomcrypt.h>

#include <cstdint>
#include <string>
#include <vector>

#include "CryptHelpers.h"
#include "CryptManager.h"
#include "LuaBinding.h"
#include "LuaManager.h"
#include "LuaReference.h"
#include "RageFile.h"
#include "RageFileManager.h"
#include "RageLog.h"
#include "RageUtil.h"

CryptManager* CRYPTMAN =
    nullptr;  // global and accessible from anywhere in our program

static const std::string PRIVATE_KEY_PATH = "Data/private.rsa";
static const std::string PUBLIC_KEY_PATH = "Data/public.rsa";
static const std::string ALTERNATE_PUBLIC_KEY_DIR = "Data/keys/";

static bool HashFile(RageFileBasic& f, unsigned char buf_hash[20], int hash) {
  hash_state hash_state;
  int ret = hash_descriptor[hash].init(&hash_state);
  ASSERT_M(ret == CRYPT_OK, error_to_string(ret));

  RString s;
  while (!f.AtEOF()) {
    s.erase();
    if (f.Read(s, 1024 * 4) == -1) {
      LOG->Warn(
          "Error reading %s: %s", f.GetDisplayPath().c_str(),
          f.GetError().c_str());
      hash_descriptor[hash].done(&hash_state, buf_hash);
      return false;
    }

    ret = hash_descriptor[hash].process(
        &hash_state, (const unsigned char*)s.data(), s.size());
    ASSERT_M(ret == CRYPT_OK, error_to_string(ret));
  }

  ret = hash_descriptor[hash].done(&hash_state, buf_hash);
  ASSERT_M(ret == CRYPT_OK, error_to_string(ret));

  return true;
}

#if defined(DISABLE_CRYPTO)
CryptManager::CryptManager() {}
CryptManager::~CryptManager() {}
void CryptManager::GenerateRSAKey(
    unsigned int keyLength, RString privFilename, RString pubFilename) {}
void CryptManager::SignFileToFile(RString sPath, RString sSignatureFile) {}
bool CryptManager::VerifyFileWithFile(
    RString sPath, RString sSignatureFile, RString sPublicKeyFile) {
  return true;
}
bool CryptManager::VerifyFileWithFile(RString sPath, RString sSignatureFile) {
  return true;
}

void CryptManager::GetRandomBytes(void* pData, int iBytes) {
  std::uint8_t* pBuf = (std::uint8_t*)pData;
  while (iBytes--) {
    *pBuf++ = (std::uint8_t)RandomInt(256);
  }
=======
    unsigned int keyLength, std::string privFilename, std::string pubFilename) {
}
void CryptManager::SignFileToFile(
    std::string sPath, std::string sSignatureFile) {}
bool CryptManager::VerifyFileWithFile(
    std::string sPath, std::string sSignatureFile, std::string sPublicKeyFile) {
  return true;
}
bool CryptManager::VerifyFileWithFile(
    std::string sPath, std::string sSignatureFile) {
  return true;
}

void CryptManager::GetRandomBytes(void* pData, int iBytes) {
  uint8_t* pBuf = (uint8_t*)pData;
  while (iBytes--) {
    *pBuf++ = (uint8_t)RandomInt(256);
  }
>>>>>>> upstream/release
        output.GetError().c_str());
    return false;
  }

  if (output.Write(buf) == -1 || output.Flush() == -1) {
    LOG->Warn(
        "WriteFile: writing %s failed: %s", file.c_str(),
        output.GetError().c_str());
    output.Close();
    FILEMAN->Remove(file);
    return false;
  }

  return true;
}

void CryptManager::GenerateRSAKey(
    unsigned int key_length, RString& private_key, RString& public_key) {
  int iRet;

  rsa_key key;
  iRet = rsa_make_key(
      &g_pPRNG->m_PRNG, g_pPRNG->m_iPRNG, key_length / 8, 65537, &key);
  if (iRet != CRYPT_OK) {
    LOG->Warn(
        "GenerateRSAKey(%i) error: %s", key_length, error_to_string(iRet));
    return;
  }

  unsigned char buf[1024];
  unsigned long iSize = sizeof(buf);
  iRet = rsa_export(buf, &iSize, PK_PUBLIC, &key);
  if (iRet != CRYPT_OK) {
    LOG->Warn("Export error: %s", error_to_string(iRet));
    return;
  }

  public_key = RString((const char*)buf, iSize);

  iSize = sizeof(buf);
  iRet = rsa_export(buf, &iSize, PK_PRIVATE, &key);
  if (iRet != CRYPT_OK) {
    LOG->Warn("Export error: %s", error_to_string(iRet));
    return;
  }

  private_key = RString((const char*)buf, iSize);
}

void CryptManager::GenerateRSAKeyToFile(
    unsigned int key_length, RString private_key_filename,
    RString public_key_filename) {
  RString private_key, public_key;
  GenerateRSAKey(key_length, private_key, public_key);

  if (!WriteFile(public_key_filename, public_key)) {
    return;
  }

  if (!WriteFile(private_key_filename, private_key)) {
    FILEMAN->Remove(private_key_filename);
    return;
  }
}

void CryptManager::SignFileToFile(RString path, RString signature_file) {
  RString sPrivFilename = PRIVATE_KEY_PATH;
  if (signature_file.empty()) {
    signature_file = path + SIGNATURE_APPEND;
  }

  RString private_key;
  if (!GetFileContents(sPrivFilename, private_key)) {
    return;
  }

  RString signature;
  if (!Sign(path, signature, private_key)) {
    return;
  }

  WriteFile(signature_file, signature);
}

bool CryptManager::Sign(
    RString path, RString& signature_out, RString private_key) {
  if (!IsAFile(path)) {
    LOG->Trace("SignFileToFile: \"%s\" doesn't exist", path.c_str());
    return false;
  }

  RageFile file;
  if (!file.Open(path)) {
    LOG->Warn(
        "SignFileToFile: open(%s) failed: %s", path.c_str(),
        file.GetError().c_str());
    return false;
  }

  RSAKeyWrapper key;
  RString error;
  if (!key.Load(private_key, error)) {
    LOG->Warn("Error loading RSA key: %s", error.c_str());
    return false;
  }

  int hash = register_hash(&sha1_desc);
  ASSERT(hash >= 0);

  unsigned char buf_hash[20];
  if (!HashFile(file, buf_hash, hash)) {
    return false;
  }

  unsigned char signature[256];
  unsigned long signature_len = sizeof(signature);

  int ret = rsa_sign_hash_ex(
      buf_hash, sizeof(buf_hash), signature, &signature_len, LTC_PKCS_1_V1_5,
      &g_pPRNG->m_PRNG, g_pPRNG->m_iPRNG, hash, 0, &key.m_Key);
  if (ret != CRYPT_OK) {
    LOG->Warn("SignFileToFile error: %s", error_to_string(ret));
    return false;
  }

  signature_out.assign((const char*)signature, signature_len);
  return true;
}

bool CryptManager::VerifyFileWithFile(RString path, RString signature_file) {
  if (VerifyFileWithFile(path, signature_file, PUBLIC_KEY_PATH)) {
    return true;
  }

  std::vector<RString> keys;
  GetDirListing(ALTERNATE_PUBLIC_KEY_DIR, keys, false, true);
  for (unsigned i = 0; i < keys.size(); ++i) {
    const RString& key = keys[i];
    LOG->Trace("Trying alternate key \"%s\" ...", key.c_str());

    if (VerifyFileWithFile(path, signature_file, key)) {
      return true;
    }
  }

  return false;
}

bool CryptManager::VerifyFileWithFile(
    RString path, RString signature_file, RString public_key_file) {
  if (signature_file.empty()) {
    signature_file = path + SIGNATURE_APPEND;
  }

  RString public_key;
  if (!GetFileContents(public_key_file, public_key)) {
    return false;
  }

  int bytes = FILEMAN->GetFileSizeInBytes(signature_file);
  if (bytes > MAX_SIGNATURE_SIZE_BYTES) {
    return false;
  }

  RString signature;
  if (!GetFileContents(signature_file, signature)) {
    return false;
  }

  RageFile file;
  if (!file.Open(path)) {
    LOG->Warn(
        "Verify: open(%s) failed: %s", path.c_str(), file.GetError().c_str());
    return false;
  }

  return Verify(file, signature, public_key);
}

bool CryptManager::Verify(
    RageFileBasic& file, RString signature, RString public_key) {
  RSAKeyWrapper key;
  RString error;
  if (!key.Load(public_key, error)) {
    LOG->Warn("Error loading RSA key: %s", error.c_str());
    return false;
  }

  int hash = register_hash(&sha1_desc);
  ASSERT(hash >= 0);

  unsigned char buf_hash[20];
  HashFile(file, buf_hash, hash);

  int match;
  int ret = rsa_verify_hash_ex(
      (const unsigned char*)signature.data(), signature.size(), buf_hash,
      sizeof(buf_hash), LTC_PKCS_1_EMSA, hash, 0, &match, &key.m_Key);

  if (ret != CRYPT_OK) {
    LOG->Warn(
        "Verify(%s) failed: %s", file.GetDisplayPath().c_str(),
        error_to_string(ret));
    return false;
  }

  if (!match) {
    LOG->Warn(
        "Verify(%s) failed: signature mismatch", file.GetDisplayPath().c_str());
    return false;
  }

  return true;
}

void CryptManager::GetRandomBytes(void* data, int bytes) {
  int ret = prng_descriptor[g_pPRNG->m_iPRNG].read(
      (unsigned char*)data, bytes, &g_pPRNG->m_PRNG);
  ASSERT(ret == bytes);
}
#endif

RString CryptManager::GetMD5ForFile(RString file_name) {
  RageFile file;
  if (!file.Open(file_name, RageFile::READ)) {
    LOG->Warn("GetMD5: Failed to open file '%s'", file_name.c_str());
    return RString();
  }
  int hash = register_hash(&md5_desc);
  ASSERT(hash >= 0);

  unsigned char digest[16];
  HashFile(file, digest, hash);

  return RString((const char*)digest, sizeof(digest));
}

RString CryptManager::GetMD5ForString(RString data) {
  unsigned char digest[16];

  int hash = register_hash(&md5_desc);

  hash_state hash_state;
  hash_descriptor[hash].init(&hash_state);
  hash_descriptor[hash].process(
      &hash_state, (const unsigned char*)data.data(), data.size());
  hash_descriptor[hash].done(&hash_state, digest);

  return RString((const char*)digest, sizeof(digest));
}

RString CryptManager::GetSHA1ForString(RString data) {
  unsigned char digest[20];

  int hash = register_hash(&sha1_desc);

  hash_state hash_state;
  hash_descriptor[hash].init(&hash_state);
  hash_descriptor[hash].process(
      &hash_state, (const unsigned char*)data.data(), data.size());
  hash_descriptor[hash].done(&hash_state, digest);

  return RString((const char*)digest, sizeof(digest));
}

RString CryptManager::GetSHA1ForFile(RString file_name) {
  RageFile file;
  if (!file.Open(file_name, RageFile::READ)) {
    LOG->Warn("GetSHA1: Failed to open file '%s'", file_name.c_str());
    return RString();
  }
  int hash = register_hash(&sha1_desc);
  ASSERT(hash >= 0);

  unsigned char digest[20];
  HashFile(file, digest, hash);

  return RString((const char*)digest, sizeof(digest));
}

RString CryptManager::GetSHA256ForString(RString data) {
  unsigned char digest[32];

  int hash = register_hash(&sha256_desc);

  hash_state hash_state;
  hash_descriptor[hash].init(&hash_state);
  hash_descriptor[hash].process(
      &hash_state, (const unsigned char*)data.data(), data.size());
  hash_descriptor[hash].done(&hash_state, digest);

  return RString((const char*)digest, sizeof(digest));
}

RString CryptManager::GetSHA256ForFile(RString file_name) {
  RageFile file;
  if (!file.Open(file_name, RageFile::READ)) {
    LOG->Warn("GetSHA256: Failed to open file '%s'", file_name.c_str());
    return RString();
  }
  int hash = register_hash(&sha256_desc);
  ASSERT(hash >= 0);

  unsigned char digest[32];
  HashFile(file, digest, hash);

  return RString((const char*)digest, sizeof(digest));
}

RString CryptManager::GetPublicKeyFileName() { return PUBLIC_KEY_PATH; }

/* Generate a version 4 random UUID. */
RString CryptManager::GenerateRandomUUID() {
  std::uint32_t buf[4];
  CryptManager::GetRandomBytes(buf, sizeof(buf));
=======
void CryptManager::GetRandomBytes(void* pData, int iBytes) {
  int iRet = prng_descriptor[g_pPRNG->m_iPRNG].read(
      (unsigned char*)pData, iBytes, &g_pPRNG->m_PRNG);
  ASSERT(iRet == iBytes);
}
#endif

std::string CryptManager::GetMD5ForFile(std::string fn) {
  RageFile file;
  if (!file.Open(fn, RageFile::READ)) {
    LOG->Warn("GetMD5: Failed to open file '%s'", fn.c_str());
    return std::string();
  }
  int iHash = register_hash(&md5_desc);
  ASSERT(iHash >= 0);

  unsigned char digest[16];
  HashFile(file, digest, iHash);

  return std::string((const char*)digest, sizeof(digest));
}

std::string CryptManager::GetMD5ForString(std::string sData) {
  unsigned char digest[16];

  int iHash = register_hash(&md5_desc);

  hash_state hash;
  hash_descriptor[iHash].init(&hash);
  hash_descriptor[iHash].process(
      &hash, (const unsigned char*)sData.data(), sData.size());
  hash_descriptor[iHash].done(&hash, digest);

  return std::string((const char*)digest, sizeof(digest));
}

std::string CryptManager::GetSHA1ForString(std::string sData) {
  unsigned char digest[20];

  int iHash = register_hash(&sha1_desc);

  hash_state hash;
  hash_descriptor[iHash].init(&hash);
  hash_descriptor[iHash].process(
      &hash, (const unsigned char*)sData.data(), sData.size());
  hash_descriptor[iHash].done(&hash, digest);

  return std::string((const char*)digest, sizeof(digest));
}

std::string CryptManager::GetSHA1ForFile(std::string fn) {
  RageFile file;
  if (!file.Open(fn, RageFile::READ)) {
    LOG->Warn("GetSHA1: Failed to open file '%s'", fn.c_str());
    return std::string();
  }
  int iHash = register_hash(&sha1_desc);
  ASSERT(iHash >= 0);

  unsigned char digest[20];
  HashFile(file, digest, iHash);

  return std::string((const char*)digest, sizeof(digest));
}

std::string CryptManager::GetSHA256ForString(std::string sData) {
  unsigned char digest[32];

  int iHash = register_hash(&sha256_desc);

  hash_state hash;
  hash_descriptor[iHash].init(&hash);
  hash_descriptor[iHash].process(
      &hash, (const unsigned char*)sData.data(), sData.size());
  hash_descriptor[iHash].done(&hash, digest);

  return std::string((const char*)digest, sizeof(digest));
}

std::string CryptManager::GetSHA256ForFile(std::string fn) {
  RageFile file;
  if (!file.Open(fn, RageFile::READ)) {
    LOG->Warn("GetSHA256: Failed to open file '%s'", fn.c_str());
    return std::string();
  }
  int iHash = register_hash(&sha256_desc);
  ASSERT(iHash >= 0);

  unsigned char digest[32];
  HashFile(file, digest, iHash);

  return std::string((const char*)digest, sizeof(digest));
}

std::string CryptManager::GetPublicKeyFileName() { return PUBLIC_KEY_PATH; }

/* Generate a version 4 random UUID. */
std::string CryptManager::GenerateRandomUUID() {
  uint32_t buf[4];
  CryptManager::GetRandomBytes(buf, sizeof(buf));
>>>>>>> upstream/release
=======
class LunaCryptManager : public Luna<CryptManager> {
 public:
  static int MD5String(T* p, lua_State* L) {
    std::string md5out;
    md5out = p->GetMD5ForString(SArg(1));
    lua_pushlstring(L, md5out.c_str(), md5out.size());
    return 1;
  }
  static int MD5File(T* p, lua_State* L) {
    std::string md5fout;
    md5fout = p->GetMD5ForFile(SArg(1));
    lua_pushlstring(L, md5fout.c_str(), md5fout.size());
    return 1;
  }
  static int SHA1String(T* p, lua_State* L) {
    std::string sha1out;
    sha1out = p->GetSHA1ForString(SArg(1));
    lua_pushlstring(L, sha1out.c_str(), sha1out.size());
    return 1;
  }
  static int SHA1File(T* p, lua_State* L) {
    std::string sha1fout;
    sha1fout = p->GetSHA1ForFile(SArg(1));
    lua_pushlstring(L, sha1fout.c_str(), sha1fout.size());
    return 1;
  }
  static int SHA256String(T* p, lua_State* L) {
    std::string sha256out;
    sha256out = p->GetSHA256ForString(SArg(1));
    lua_pushlstring(L, sha256out.c_str(), sha256out.size());
    return 1;
  }
  static int SHA256File(T* p, lua_State* L) {
    std::string sha256fout;
    sha256fout = p->GetSHA256ForFile(SArg(1));
    lua_pushlstring(L, sha256fout.c_str(), sha256fout.size());
    return 1;
  }
  static int GenerateRandomUUID(T* p, lua_State* L) {
    std::string uuidOut;
    uuidOut = p->GenerateRandomUUID();
    lua_pushlstring(L, uuidOut.c_str(), uuidOut.size());
    return 1;
  }
>>>>>>> upstream/release
=======
>>>>>>> Stashed changes

  LunaCryptManager() {
    ADD_METHOD(MD5String);
    ADD_METHOD(MD5File);
    ADD_METHOD(SHA1String);
    ADD_METHOD(SHA1File);
    ADD_METHOD(SHA256String);
    ADD_METHOD(SHA256File);
    ADD_METHOD(GenerateRandomUUID);
  }
};

LUA_REGISTER_CLASS(CryptManager)

// lua end

/*
 * (c) 2004-2007 Chris Danford, Glenn Maynard
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
