#ifndef ENUM_INCLUDED
#define ENUM_INCLUDED

#include <vector>
#include <string>
#include <map>
#include <inja/inja.hpp>

typedef std::string string;
enum HTTP_METHODS
{
	HTTP_METHOD_GET = 0,
	HTTP_METHOD_POST,
	HTTP_METHOD_UNDEFINED = -1

};
enum ACCEPT_LANGUAGE
{
	ACCEPT_LANGUAGE_EN_US,
	ACCEPT_LANGUAGE_EN_Q_0_5
};
enum HTTP_VERSION
{
	HTTP_VERSION_HTTP1 = 1,
	HTTP_VERSION_HTTP2 = 2,
	HTTP_VERSION_UNDEFINED = -1

};
enum CONNECTION_TYPE
{
	CONNECTION_TYPE_KEEP_ALIVE = 1,
	CONNECTION_TYPE_CLOSE = 2,
	CONNECTION_TYPE_UNDEFINED = -1
};
enum ENCODING
{
	ENCODING_UTF8,
	ENCODING_UNDEFINED = -1
};
enum MIME_TYPE
{
	MIME_TYPE_text_html = 0,
	MIME_TYPE_image_bmp,
	MIME_TYPE_text_css,
	MIME_TYPE_application_gzip,
	MIME_TYPE_image_jpeg,
	MIME_TYPE_text_javascript,
	MIME_TYPE_application_json,
	MIME_TYPE_video_mp4,
	MIME_TYPE_image_png,
	MIME_TYPE_application_pdf,
	MIME_TYPE_image_svg_xml,
	MIME_TYPE_text_plain,
	MIME_TYPE_image_webp,
	MIME_TYPE_application_xhtml_xml,
	MIME_TYPE_application_xml,
	MIME_TYPE_video_webm,
	MIME_TYPE_SELF,
	MIME_TYPE_octet_stream,
	MIME_TYPE_video_mkv,
	MIME_TYPE_UNDEFINED = -1
};
enum HTTP_STATUS_CODE
{
	HTTP_STATUS_CODE_OK = 200,
	HTTP_STATUS_CODE_NO_CONTENT = 204,
	HTTP_STATUS_CODE_Not_Modified = 304,
	HTTP_STATUS_CODE_Found = 302,
	HTTP_STATUS_CODE_Not_Found = 404,
	HTTP_STATUS_CODE_Bad_Request = 400,
	HTTP_STATUS_CODE_Internal_Server_Error = 500,
	HTTP_STATUS_CODE_UNDEFINED = -1
};
enum CONTENT_ENCODING
{
	CONTENT_ENCODING_GZIP = 0,
	CONTENT_ENCODING_COMPRESS,
	CONTENT_ENCODING_DEFLATE,
	CONTENT_ENCODING_BR,
	CONTENT_ENCODING_CHUNKED,
	CONTENT_ENCODING_UNDEFINED = -1
};

enum TYPE
{
	TEXT_TYPE = 0,
	FILE_TYPE = 1,
	UNDEFINED_TYPE = -1
};

typedef struct __MULTIPART_DATA
{
	bool isFile;
	string value;
	string name;
	string content_type;
	string content_disposition;
	string isfilename;
} MULTIPART_DATA;
typedef enum __HTTP_PAYLOAD_TYPE
{
	PAYLOAD_TYPE_NO_PAYLOAD = -1,
	PAYLOAD_TYPE_GET_PAYLOAD = 0,
	PAYLOAD_TYPE_URL_ENCODED_PAYLOAD = 1,

	PAYLOAD_TYPE_MULTIPART_PAYLOAD = 2,

} PAYLOAD_TYPE;

typedef struct __HTTP_HEADER_OBJECT
{
	HTTP_METHODS method;
	std::string path;
	std::multimap<std::string, std::string> payload;
	std::vector<MULTIPART_DATA> multipart_data;

	std::string host;
	CONNECTION_TYPE connection_type;
	std::string cookieString;
	std::vector<CONTENT_ENCODING> accept_encoding;
	std::vector<std::string> accept;
	std::vector<std::string> accept_language;
	std::string user_agent;
	std::string sec_fetch_mode;
	std::string sec_fetch_site;
	std::string sec_fetch_dest;
	std::string cache_control;
	HTTP_VERSION version;
	std::string contentLength;
	std::string upgrade_insecure_request;
	std::string content_type;
	PAYLOAD_TYPE payloadType;

} HTTP_HEADER, *PHTTP_HEADER;

typedef struct _CONTENT_TYPE
{
	std::string content_type;
	std::string boundry;
} CONTENT_TYPE;
typedef enum __REDIRECT_TYPE
{
	REDIRECT_TYPE_MULTIPLE_CHOICE = 300,
	REDIRECT_TYPE_MOVED_PERMANENTLY = 301,
	REDIRECT_TYPE_FOUND = 302,
	REDIRECT_TYPE_SEE_OTHER = 303,
	REDIRECT_TYPE_NOT_MODIFIED = 304,
	REDIRECT_TYPE_TEMPORERY_REDIRECT = 307,
	REDIRECT_TYPE_PERMANENT_REDIRECT = 308

} REDIRECT_TYPE;
typedef enum __SAMESITE_TYPE
{
	SAMESITE_TYPE_STRICT = 1,
	SAMESITE_TYPE_LAX,
	SAMESITE_TYPE_NONE,
	SAMESITE_TYPE_UNDEFINED = -1
} SAMESITE_TYPE;

typedef struct __COOKIE
{
	std::string name;
	std::string value;
	std::string expiry;
	bool secure;
	bool httpOnly;
	int maxAge;
	std::string domain;
	std::string path;
	SAMESITE_TYPE sameSite;
} COOKIE;

typedef struct __RESPONSE
{
	std::string val;
	std::string header;
	bool type;
	inja::json json;

} RESPONSE;
class httpResponse;
class httpRequest;
typedef struct __RESPONSE_STRUCT
{
	std::shared_ptr<httpRequest> req;
	std::shared_ptr<httpResponse> res;
	RESPONSE response;
} RESPONSE_DATA;

typedef struct __HTTP_RESPONSE_OBJECT
{

	HTTP_VERSION version;
	HTTP_STATUS_CODE status_code;
	std::string host;
	CONNECTION_TYPE connection_type;
    std::vector<COOKIE> cookies;
	long long contentLength;
	std::string content_type;
	std::string location;
	std::multimap<string, string> others;
} HTTP_RESPONSE;

#define LOCALHOST "127.0.0.1"
#endif