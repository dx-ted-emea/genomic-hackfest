//  -----------------------------------------------------------------------------------------
// this file is part of work done during Azure Genomics hackfest in Helsinki, June 2016
// started by abokov, but other folks are also contributed

#include "genomic-storage.h"

#include "cpprest/filestream.h"
#include "cpprest/containerstream.h"

using namespace azure;
using namespace storage;

namespace genomic {

#define __DEBUG_GENOMIC__  1

GenomicStorage::GenomicStorage() {
// do nothing, RFU actually
};

GenomicStorage::~GenomicStorage() {
// clean up after all
};

bool GenomicStorage::Init(AzureStorageParams &param) {
try {
	if (param.Validate() == false) {
		error_descr = "Wrong length of one of params : "+std::string(param.GetDescription())+"\n";
		return false;
	}
#ifdef __DEBUG_GENOMIC__
        std::cout<<"GenomicStorage::Init("<<param.GetDescription()<<")\n";
#endif
	storage_connection_string="DefaultEndpointsProtocol=https;AccountName="+param.account_name+";AccountKey="+param.account_key;

	storage_account = azure::storage::cloud_storage_account::parse(storage_connection_string);
	blob_client = storage_account.create_cloud_blob_client();
	blob_container = blob_client.get_container_reference(param.container_name);
	

#ifdef __DEBUG_GENOMIC__
	std::cout<<"storage_connection_string = \""<<storage_connection_string<<"\"\n";
#endif


} catch (const std::invalid_argument &ex )  {
	error_descr = "Exception std::invalid_argument caught : " + std::string(ex.what());
#ifdef __DEBUG_GENOMIC__
	std::cout<<error_descr<<"\n";
#endif
	return false;
} catch  (const std::exception &ex )  {
        error_descr = "Exception std::exception caught : " + std::string(ex.what());
#ifdef __DEBUG_GENOMIC__
        std::cout<<error_descr<<"\n";
#endif
	return false;
} catch (...) {
        error_descr = "Unknown exception caught ";
#ifdef __DEBUG_GENOMIC__
        std::cout<<error_descr<<"\n";
#endif
	return false;
} 

return true;
};


bool GenomicStorage::SaveFileToBlob(const char *blob_name, const char *file_name, bool is_public) {
try {
#ifdef __DEBUG_GENOMIC__
        std::cout<<"GenomicStorage::SaveFileToBlob(blob_name=\""<<blob_name<<"\"file_name=\""<<file_name<<"\",is_public="<<(int)is_public<<")\n";
#endif


        blob_container.create_if_not_exists();

	//setting up permissions 
	azure::storage::blob_container_permissions permissions;
	if (is_public)
		permissions.set_public_access(azure::storage::blob_container_public_access_type::blob);
	else
		permissions.set_public_access(azure::storage::blob_container_public_access_type::off);
	blob_container.upload_permissions(permissions);

	
	concurrency::streams::istream input_stream = concurrency::streams::file_stream<uint8_t>::open_istream(file_name).get();


	azure::storage::cloud_block_blob blob1 = blob_container.get_block_blob_reference(blob_name);
#ifdef __DEBUG_GENOMIC__
        std::cout<<"blob_uri="<<blob1.uri().primary_uri().to_string()<<"\n";
#endif

	blob1.upload_from_stream(input_stream);
	input_stream.close().wait();



} catch (const std::invalid_argument &ex )  {
        error_descr = "Exception std::invalid_argument caught : " + std::string(ex.what());
#ifdef __DEBUG_GENOMIC__
        std::cout<<error_descr<<"\n";
#endif
        return false;
} catch  (const std::exception &ex )  {
        error_descr = "Exception std::exception caught : " + std::string(ex.what());
#ifdef __DEBUG_GENOMIC__
        std::cout<<error_descr<<"\n";
#endif
        return false;
} catch (...) {
        error_descr = "Unknown exception caught ";
#ifdef __DEBUG_GENOMIC__
        std::cout<<error_descr<<"\n";
#endif
        return false;
} 
return true;
};

bool GenomicStorage::SaveBlobToFile(const char *blob_name, const char *file_name) {
try {
	return false;

} catch (const std::invalid_argument &ex )  {
        error_descr = "Exception std::invalid_argument caught : " + std::string(ex.what());
#ifdef __DEBUG_GENOMIC__
        std::cout<<error_descr<<"\n";
#endif
        return false;
} catch  (const std::exception &ex )  {
        error_descr = "Exception std::exception caught : " + std::string(ex.what());
#ifdef __DEBUG_GENOMIC__
        std::cout<<error_descr<<"\n";
#endif
        return false;
} catch (...) {
        error_descr = "Unknown exception caught ! ";
#ifdef __DEBUG_GENOMIC__
        std::cout<<error_descr<<"\n";
#endif
        return false;
} 
return true;
};




} // namespace azure::storage::samples


