#include "core/stdafx.h"
#include "tier0/cvar.h"
#include "mathlib/adler32.h"
#include "mathlib/crc32.h"
#include "engine/sys_utils.h"
#include "vpklib/packedstore.h"

/***********************************************************************
* ██████╗ ██████╗     ██╗   ██╗██████╗ ██╗  ██╗    ██╗     ██╗██████╗  *
* ██╔══██╗╚════██╗    ██║   ██║██╔══██╗██║ ██╔╝    ██║     ██║██╔══██╗ *
* ██████╔╝ █████╔╝    ██║   ██║██████╔╝█████╔╝     ██║     ██║██████╔╝ *
* ██╔══██╗██╔═══╝     ╚██╗ ██╔╝██╔═══╝ ██╔═██╗     ██║     ██║██╔══██╗ *
* ██║  ██║███████╗     ╚████╔╝ ██║     ██║  ██╗    ███████╗██║██████╔╝ *
* ╚═╝  ╚═╝╚══════╝      ╚═══╝  ╚═╝     ╚═╝  ╚═╝    ╚══════╝╚═╝╚═════╝  *
***********************************************************************/

//-----------------------------------------------------------------------------
// Purpose: initialize parameters for decompression algorithm
//-----------------------------------------------------------------------------
void CPackedStore::InitLzParams()
{
	/*| PARAMETERS ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
	m_lzDecompParams.m_dict_size_log2   = RVPK_DICT_SIZE;
	m_lzDecompParams.m_decompress_flags = LZHAM_DECOMP_FLAG_OUTPUT_UNBUFFERED | LZHAM_DECOMP_FLAG_COMPUTE_CRC32;
	m_lzDecompParams.m_struct_size      = sizeof(lzham_decompress_params);
}

//-----------------------------------------------------------------------------
// Purpose: obtains archive chunk path for specific file
//-----------------------------------------------------------------------------
std::string CPackedStore::GetPackChunkFile(std::string svPackDirFile, int iArchiveIndex)
{
	/*| ARCHIVES ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
	std::string svPackChunkFile = StripLocalePrefix(svPackDirFile);
	std::ostringstream oss;

	oss << std::setw(3) << std::setfill('0') << iArchiveIndex;
	std::string svPackChunkIndex = "pak000_" + oss.str();

	string_replace(svPackChunkFile, "pak000_dir", svPackChunkIndex);
	return svPackChunkFile;
}

//-----------------------------------------------------------------------------
// Purpose: returns pupulated pack dir struct for specified pack dir file
//-----------------------------------------------------------------------------
vpk_dir_h CPackedStore::GetPackDirFile(std::string svPackDirFile)
{
	/*| PACKDIRFILE |||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
	std::regex rgArchiveRegex("pak000_([0-9]{3})");
	std::smatch smRegexMatches;

	std::regex_search(svPackDirFile, smRegexMatches, rgArchiveRegex);

	if (smRegexMatches.size() != 0)
	{
		string_replace(svPackDirFile, smRegexMatches[0], "pak000_dir");

		for (int i = 0; i < LANGUAGE_PACKS; i++)
		{
			if (strstr(svPackDirFile.c_str(), DIR_LIBRARY_PREFIX[i].c_str()))
			{
				for (int j = 0; j < LIBRARY_PACKS; j++)
				{
					if (strstr(svPackDirFile.c_str(), DIR_LIBRARY_PREFIX[j].c_str()))
					{
						std::string svPackDirPrefix = DIR_LOCALE_PREFIX[i] + DIR_LOCALE_PREFIX[i];
						string_replace(svPackDirFile, DIR_LOCALE_PREFIX[i].c_str(), svPackDirPrefix.c_str());
						goto escape;
					}
				}
			}
		}escape:;
	}

	vpk_dir_h vpk_dir(svPackDirFile);
	return vpk_dir;
}

//-----------------------------------------------------------------------------
// Purpose: obtains and returns the entry block to the vector
//-----------------------------------------------------------------------------
std::vector<vpk_entry_block> CPackedStore::GetEntryBlocks(CIOStream* reader)
{
	/*| ENTRYBLOCKS |||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
	std::string svName, svPath, svExtension;
	std::vector<vpk_entry_block> vBlocks;
	while (!(svExtension = reader->readString()).empty())
	{
		while (!(svPath = reader->readString()).empty())
		{
			while (!(svName = reader->readString()).empty())
			{
				std::string svFilePath = FormatBlockPath(svName, svPath, svExtension);
				vBlocks.push_back(vpk_entry_block(reader, svFilePath));
			}
		}
	}
	return vBlocks;
}

//-----------------------------------------------------------------------------
// Purpose: formats the entry block path
//-----------------------------------------------------------------------------
std::string CPackedStore::FormatBlockPath(std::string svName, std::string svPath, std::string svExtension)
{
	if (!svPath.empty())
	{
		svPath += "\\";
	}
	return svPath + svName + "." + svExtension;
}

//-----------------------------------------------------------------------------
// Purpose: strips locale prefix from file path
//-----------------------------------------------------------------------------
std::string CPackedStore::StripLocalePrefix(std::string svPackDirFile)
{
	std::filesystem::path fspPackDirFile(svPackDirFile);
	std::string svFileName = fspPackDirFile.filename().u8string();

	for (int i = 0; i < LANGUAGE_PACKS; i++)
	{
		if (strstr(svFileName.c_str(), DIR_LOCALE_PREFIX[i].c_str()))
		{
			string_replace(svFileName, DIR_LOCALE_PREFIX[i].c_str(), "");
			goto escape;
		}
	}escape:;
	return svFileName;
}

//-----------------------------------------------------------------------------
// Purpose: validates extraction result with precomputed ADLER32 hash
//-----------------------------------------------------------------------------
void CPackedStore::ValidateAdler32PostDecomp(std::string svAssetFile)
{
	uint32_t adler_init = {};
	std::ifstream istream(svAssetFile, std::fstream::binary);

	istream.seekg(0, std::fstream::end);
	m_vHashBuffer.resize(istream.tellg());
	istream.seekg(0, std::fstream::beg);
	istream.read((char*)m_vHashBuffer.data(), m_vHashBuffer.size());

	m_nAdler32 = adler32::update(adler_init, m_vHashBuffer.data(), m_vHashBuffer.size());

	if (m_nAdler32 != m_nAdler32_Internal)
	{
		Sys_Print(SYS_DLL::FS, "Warning: ADLER32 checksum mismatch for entry '%s' computed value '0x%luX' doesn't match expected value '0x%luX'!\n", svAssetFile.c_str(), m_nAdler32, m_nAdler32_Internal);
		m_nAdler32          = 0;
		m_nAdler32_Internal = 0;
	}

	istream.close();
	m_vHashBuffer.clear();
}

//-----------------------------------------------------------------------------
// Purpose: validates extraction result with precomputed CRC32 hash
//-----------------------------------------------------------------------------
void CPackedStore::ValidateCRC32PostDecomp(std::string svDirAsset)
{
	uint32_t crc32_init = {};
	std::ifstream istream(svDirAsset, std::fstream::binary);

	istream.seekg(0, std::fstream::end);
	m_vHashBuffer.resize(istream.tellg());
	istream.seekg(0, std::fstream::beg);
	istream.read((char*)m_vHashBuffer.data(), m_vHashBuffer.size());

	m_nCrc32 = crc32::update(crc32_init, m_vHashBuffer.data(), m_vHashBuffer.size());

	if (m_nCrc32 != m_nCrc32_Internal)
	{
		Sys_Print(SYS_DLL::FS, "Warning: CRC32 checksum mismatch for entry '%s' computed value '0x%luX' doesn't match expected value '0x%luX'!\n", svDirAsset.c_str(), m_nCrc32, m_nCrc32_Internal);
		m_nCrc32          = 0;
		m_nCrc32_Internal = 0;
	}

	istream.close();
	m_vHashBuffer.clear();
}

//-----------------------------------------------------------------------------
// Purpose: extracts all files from specified vpk file
//-----------------------------------------------------------------------------
void CPackedStore::UnpackAll(vpk_dir_h vpk_dir, std::string svPathOut)
{
	for (int i = 0; i < vpk_dir.m_vsvArchives.size(); i++)
	{
		std::filesystem::path fspVpkPath(vpk_dir.m_svDirPath);
		std::string svPath = fspVpkPath.parent_path().u8string() + "\\" + vpk_dir.m_vsvArchives[i];
		std::ifstream packChunkStream(svPath, std::ios_base::binary); // Create stream to read from each archive.

		for ( vpk_entry_block block : vpk_dir.m_vvEntryBlocks)
		{
			if (block.m_iArchiveIndex != i)
			{
				// Continue if block archive index is not part of the extracting archive chunk index.
				goto cont;
			}
			else
			{
				std::string svFilePath = create_directories(svPathOut + "\\" + block.m_svBlockPath);
				std::ofstream outFileStream(svFilePath, std::ios_base::binary | std::ios_base::out);

				if (!outFileStream.is_open())
				{
					Sys_Print(SYS_DLL::FS, "Error: unable to access file '%s'!\n", svFilePath.c_str());
				}
				outFileStream.clear(); // Make sure file is empty before writing.
				for (vpk_entry_h entry : block.m_vvEntries)
				{
					char* pCompressedData = new char[entry.m_nCompressedSize];
					memset(pCompressedData, 0, entry.m_nCompressedSize); // Compressed region.

					packChunkStream.seekg(entry.m_nArchiveOffset);       // Seek to entry offset in archive.
					packChunkStream.read(pCompressedData, entry.m_nCompressedSize); // Read compressed data from archive.

					if (entry.m_bIsCompressed)
					{
						lzham_uint8* pLzOutputBuf = new lzham_uint8[entry.m_nUncompressedSize];
						m_lzDecompStatus = lzham_decompress_memory(&m_lzDecompParams, pLzOutputBuf, (size_t*)&entry.m_nUncompressedSize, (lzham_uint8*)pCompressedData, entry.m_nCompressedSize, &m_nAdler32_Internal, &m_nCrc32_Internal);

						if (fs_packedstore_entryblock_stats->m_iValue > 0)
						{
							Sys_Print(SYS_DLL::FS, "--------------------------------------------------------------\n");
							Sys_Print(SYS_DLL::FS, "] Block path            : '%s'\n", block.m_svBlockPath.c_str());
							Sys_Print(SYS_DLL::FS, "] Entry count           : '%llu'\n", block.m_vvEntries.size());
							Sys_Print(SYS_DLL::FS, "] Compressed size       : '%llu'\n", entry.m_nCompressedSize);
							Sys_Print(SYS_DLL::FS, "] Uncompressed size     : '%llu'\n", entry.m_nUncompressedSize);
							Sys_Print(SYS_DLL::FS, "] Static CRC32 hash     : '0x%luX'\n", block.m_nCrc32);
							Sys_Print(SYS_DLL::FS, "] Computed CRC32 hash   : '0x%luX'\n", m_nCrc32_Internal);
							Sys_Print(SYS_DLL::FS, "] Computed ADLER32 hash : '0x%luX'\n", m_nAdler32_Internal);
							Sys_Print(SYS_DLL::FS, "--------------------------------------------------------------\n");
						}

						if (block.m_vvEntries.size() == 1) // Internal checksum can only match block checksum if entry size is 1.
						{
							if (block.m_nCrc32 != m_nCrc32_Internal)
							{
								Sys_Print(SYS_DLL::FS, "Warning: CRC32 checksum mismatch for entry '%s' computed value '0x%luX' doesn't match expected value '0x%luX'!\n", block.m_svBlockPath.c_str(), m_nCrc32_Internal, block.m_nCrc32);
							}
						}
						else
						{
							// Set internal hash to precomputed entry hash for post decompress validation.
							m_nCrc32_Internal = block.m_nCrc32;
							m_nEntryCount++;
						}

						if (m_lzDecompStatus != lzham_decompress_status_t::LZHAM_DECOMP_STATUS_SUCCESS)
						{
							Sys_Print(SYS_DLL::FS, "Error: failed decompression for an entry within block '%s' in archive '%d'!\n", block.m_svBlockPath.c_str(), i);
							Sys_Print(SYS_DLL::FS, "'lzham_decompress_memory_func' returned with status '%d'.\n", m_lzDecompStatus);
						}
						else
						{
							// If successfully decompressed, write to file.
							outFileStream.write((char*)pLzOutputBuf, entry.m_nUncompressedSize);
							delete[] pLzOutputBuf, pCompressedData;
						}
					}
					else
					{
						// If not compressed, write raw data into output file.
						outFileStream.write(pCompressedData, entry.m_nUncompressedSize);
					}
				}
				outFileStream.close();
				if (m_nEntryCount == block.m_vvEntries.size()) // Only validate after last entry in block had been written.
				{
					ValidateCRC32PostDecomp(svFilePath);
					//ValidateAdler32PostDecomp(svFilePath);
					m_nEntryCount       = 0;
				}
			}cont:;
		}
		packChunkStream.close();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 'vpk_entry_block' constructor
//-----------------------------------------------------------------------------
vpk_entry_block::vpk_entry_block(CIOStream* reader, std::string svPath)
{
	std::replace(svPath.begin(), svPath.end(), '/', '\\'); // Flip forward slashes in filepath to windows-style backslash.

	this->m_svBlockPath = svPath; // Set path of block.
	reader->read<uint32_t>(this->m_nCrc32);        //
	reader->read<uint16_t>(this->m_nPreloadBytes); //
	reader->read<uint16_t>(this->m_iArchiveIndex); //

	do // Loop through all entries in the block and push them to the vector.
	{
		vpk_entry_h entry(reader);
		this->m_vvEntries.push_back(entry);
	} while (reader->readR<uint16_t>() != 65535);
}

//-----------------------------------------------------------------------------
// Purpose: 'vpk_entry_h' constructor
//-----------------------------------------------------------------------------
vpk_entry_h::vpk_entry_h(CIOStream* reader)
{
	reader->read<uint32_t>(this->m_nEntryFlags);       //
	reader->read<uint16_t>(this->m_nTextureFlags);     //
	reader->read<uint64_t>(this->m_nArchiveOffset);    //
	reader->read<uint64_t>(this->m_nCompressedSize);   //
	reader->read<uint64_t>(this->m_nUncompressedSize); //
	this->m_bIsCompressed = (this->m_nCompressedSize != this->m_nUncompressedSize);
}

//-----------------------------------------------------------------------------
// Purpose: 'vpk_dir_h' constructor
//-----------------------------------------------------------------------------
vpk_dir_h::vpk_dir_h(std::string svPath)
{
	CIOStream reader;

	reader.open(svPath, eStreamFileMode::READ);
	reader.read<uint32_t>(this->m_nFileMagic);

	if (this->m_nFileMagic != RVPK_DIR_MAGIC)
	{
		Sys_Print(SYS_DLL::FS, "Error: vpk_dir file '%s' has invalid magic!\n", svPath.c_str());
		return;
	}

	reader.read<uint16_t>(this->m_nMajorVersion); //
	reader.read<uint16_t>(this->m_nMinorVersion); //
	reader.read<uint32_t>(this->m_nTreeSize);     //
	reader.read<uint32_t>(this->m_nFileDataSize); //

	Sys_Print(SYS_DLL::FS, "______________________________________________________________\n");
	Sys_Print(SYS_DLL::FS, "] HEADER_DETAILS ---------------------------------------------\n");
	Sys_Print(SYS_DLL::FS, "] File Magic     : '%lu'\n", this->m_nFileMagic);
	Sys_Print(SYS_DLL::FS, "] Major Version  : '%hu'\n", (this->m_nMajorVersion));
	Sys_Print(SYS_DLL::FS, "] Minor Version  : '%hu'\n", (this->m_nMinorVersion));
	Sys_Print(SYS_DLL::FS, "] Tree Size      : '%lu'\n", this->m_nTreeSize);
	Sys_Print(SYS_DLL::FS, "] File Data Size : '%lu'\n", this->m_nFileDataSize);

	this->m_vvEntryBlocks = g_pPackedStore->GetEntryBlocks(&reader);
	this->m_svDirPath = svPath; // Set path to vpk_dir file.

	for (vpk_entry_block block : this->m_vvEntryBlocks)
	{
		if (block.m_iArchiveIndex > this->m_iArchiveCount)
		{
			this->m_iArchiveCount = block.m_iArchiveIndex;
		}
	}

	Sys_Print(SYS_DLL::FS, "______________________________________________________________\n");
	Sys_Print(SYS_DLL::FS, "] PACK_CHUNKS ------------------------------------------------\n");

	for (int i = 0; i < this->m_iArchiveCount + 1; i++)
	{
		std::string svArchivePath = g_pPackedStore->GetPackChunkFile(svPath, i);
		Sys_Print(SYS_DLL::FS, "] '%s\n", svArchivePath.c_str());
		this->m_vsvArchives.push_back(svArchivePath);
	}
}
///////////////////////////////////////////////////////////////////////////////
CPackedStore* g_pPackedStore = new CPackedStore();
