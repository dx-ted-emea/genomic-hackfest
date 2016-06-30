// -----------------------------------------------------------------------------------------
// this file is part of work done during Azure Genomics hackfest in Helsinki, June 2016
// started by abokov, but other folks are also contributed

#include "genomic-storage.h"


using namespace genomic;



bool parse_args(int argc, const char * argv[], AzureStorageParams &params);

int main(int argc, const char *argv[])
{
	AzureStorageParams params;
	if (parse_args(argc, argv, params) == false) {
		std::cerr<<"Usage: genomicstorage   _name_of_account_   _account_key_    _container_name_    _blob_name_     _upload_file_\n";
		return 1;
	};
	GenomicStorage my_storage = GenomicStorage();

	if (my_storage.Init(params) == false ) {
		std::cerr<<"Error at storage init\n command line: ";
		for (int i=0;i<argc;++i) std::cerr<<argv[i]<<" ";
		std::cerr<<"\n Description: "<<my_storage.GetErrorDescription()<<"\n";
		return 2;
	}

    return 0;
}

bool parse_args(int argc, const char * argv[], AzureStorageParams &params) {
//it could be something smarter there, but now use that hummer
	if (argc!=6)
		return false;
	params.account_name = argv[1];
	params.account_key = argv[2];
	params.container_name = argv[3];
	params.blob_name =  argv[4];
	params.upload_file = argv[5];
	return true;
};




