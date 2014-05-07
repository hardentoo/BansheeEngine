#include "CmGpuProgramImporter.h"
#include "CmPath.h"
#include "CmDataStream.h"
#include "CmFileSystem.h"
#include "CmGpuProgramImportOptions.h"
#include "CmHighLevelGpuProgram.h"

namespace BansheeEngine
{
	bool GpuProgramImporter::isExtensionSupported(const WString& ext) const
	{
		if(ext == L"gpuprog")
			return true;

		return false;
	}

	bool GpuProgramImporter::isMagicNumberSupported(const UINT8* magicNumPtr, UINT32 numBytes) const
	{
		// Gpu program files are plain-text so no magic number
		return true;
	}

	ResourcePtr GpuProgramImporter::import(const Path& filePath, ConstImportOptionsPtr importOptions)
	{
		WString ext = filePath.getWExtension();
		ext = ext.substr(1, ext.size() - 1); // Remove the .

		DataStreamPtr stream = FileSystem::openFile(filePath);
		String shaderSource = stream->getAsString();

		const GpuProgramImportOptions* gpuProgImportOptions = static_cast<const GpuProgramImportOptions*>(importOptions.get());

		String entryPoint = gpuProgImportOptions->getEntryPoint();
		GpuProgramProfile profile = gpuProgImportOptions->getProfile();
		String language = gpuProgImportOptions->getLanguage();
		GpuProgramType gptype = gpuProgImportOptions->getType();
		Vector<HGpuProgInclude> includes = gpuProgImportOptions->getIncludes();

		HighLevelGpuProgramPtr gpuProgram = HighLevelGpuProgram::_createPtr(shaderSource, entryPoint, language, gptype, profile, &includes);
		gpuProgram->synchronize();

		WString fileName = filePath.getWFilename(false);
		gpuProgram->setName(toString(fileName));

		return gpuProgram;
	}

	ImportOptionsPtr GpuProgramImporter::createImportOptions() const
	{
		return cm_shared_ptr<GpuProgramImportOptions, PoolAlloc>();
	}
}