/*
 * http.h — A lightweight single-header HTTP library for C
 *
 * This library provides both HTTP client and HTTP server functionality
 * with zero external dependencies. It is designed to be simple, minimal,
 * and portable across POSIX-compliant systems.
 *
 * Features:
 *   - HTTP Client:
 *       • Build and send HTTP requests (GET, POST, etc.)
 *       • Parse HTTP responses
 *       • Header and body management
 *
 *   - HTTP Server:
 *       • Create lightweight HTTP servers
 *       • Register request handlers by route
 *       • Built-in error handling
 *
 *   - Utilities:
 *       • String builder for efficient text operations
 *       • File reading helpers
 *       • Complete set of HTTP status codes and Content-Type definitions
 *
 * Usage:
 *   - To include declarations:
 *       #include "http.h"
 *
 *   - To include implementation:
 *       #define HTTP_IMPLEMENTATION
 *       #include "http.h"
 *
 * License:
 *   MIT License
 *
 */

#ifndef HTTP_H
#define HTTP_H

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <netdb.h>

#define UNUSED(x) (void)(x)

#define PROTOCOL "HTTP/1.1"

// 1xx - Informational
#define STATUS_CONTINUE 100
#define STATUS_SWITCHING_PROTOCOLS 101
#define STATUS_PROCESSING 102
#define STATUS_EARLY_HINTS 103

// 2xx - Success
#define STATUS_OK 200
#define STATUS_CREATED 201
#define STATUS_ACCEPTED 202
#define STATUS_NON_AUTHORITATIVE_INFORMATION 203
#define STATUS_NO_CONTENT 204
#define STATUS_RESET_CONTENT 205
#define STATUS_PARTIAL_CONTENT 206
#define STATUS_MULTI_STATUS 207
#define STATUS_ALREADY_REPORTED 208
#define STATUS_IM_USED 226

// 3xx - Redirection
#define STATUS_MULTIPLE_CHOICES 300
#define STATUS_MOVED_PERMANENTLY 301
#define STATUS_FOUND 302
#define STATUS_SEE_OTHER 303
#define STATUS_NOT_MODIFIED 304
#define STATUS_USE_PROXY 305
#define STATUS_TEMPORARY_REDIRECT 307
#define STATUS_PERMANENT_REDIRECT 308

// 4xx - Client Error
#define STATUS_BAD_REQUEST 400
#define STATUS_UNAUTHORIZED 401
#define STATUS_PAYMENT_REQUIRED 402
#define STATUS_FORBIDDEN 403
#define STATUS_NOT_FOUND 404
#define STATUS_METHOD_NOT_ALLOWED 405
#define STATUS_NOT_ACCEPTABLE 406
#define STATUS_PROXY_AUTHENTICATION_REQUIRED 407
#define STATUS_REQUEST_TIMEOUT 408
#define STATUS_CONFLICT 409
#define STATUS_GONE 410
#define STATUS_LENGTH_REQUIRED 411
#define STATUS_PRECONDITION_FAILED 412
#define STATUS_PAYLOAD_TOO_LARGE 413
#define STATUS_URI_TOO_LONG 414
#define STATUS_UNSUPPORTED_MEDIA_TYPE 415
#define STATUS_RANGE_NOT_SATISFIABLE 416
#define STATUS_EXPECTATION_FAILED 417
#define STATUS_IM_A_TEAPOT 418
#define STATUS_MISDIRECTED_REQUEST 421
#define STATUS_UNPROCESSABLE_ENTITY 422
#define STATUS_LOCKED 423
#define STATUS_FAILED_DEPENDENCY 424
#define STATUS_TOO_EARLY 425
#define STATUS_UPGRADE_REQUIRED 426
#define STATUS_PRECONDITION_REQUIRED 428
#define STATUS_TOO_MANY_REQUESTS 429
#define STATUS_REQUEST_HEADER_FIELDS_TOO_LARGE 431
#define STATUS_UNAVAILABLE_FOR_LEGAL_REASONS 451

// 5xx - Server Error
#define STATUS_INTERNAL_SERVER_ERROR 500
#define STATUS_NOT_IMPLEMENTED 501
#define STATUS_BAD_GATEWAY 502
#define STATUS_SERVICE_UNAVAILABLE 503
#define STATUS_GATEWAY_TIMEOUT 504
#define STATUS_HTTP_VERSION_NOT_SUPPORTED 505
#define STATUS_VARIANT_ALSO_NEGOTIATES 506
#define STATUS_INSUFFICIENT_STORAGE 507
#define STATUS_LOOP_DETECTED 508
#define STATUS_NOT_EXTENDED 510
#define STATUS_NETWORK_AUTHENTICATION_REQUIRED 511

// Text types
#define CONTENT_TYPE_TEXT_PLAIN "text/plain"
#define CONTENT_TYPE_TEXT_HTML "text/html"
#define CONTENT_TYPE_TEXT_CSS "text/css"
#define CONTENT_TYPE_TEXT_JAVASCRIPT "text/javascript"
#define CONTENT_TYPE_APPLICATION_JAVASCRIPT "application/javascript"

// Application types
#define CONTENT_TYPE_APPLICATION_JSON "application/json"
#define CONTENT_TYPE_APPLICATION_XML "application/xml"
#define CONTENT_TYPE_APPLICATION_X_WWW_FORM "application/x-www-form-urlencoded"
#define CONTENT_TYPE_APPLICATION_OCTET_STREAM "application/octet-stream"
#define CONTENT_TYPE_APPLICATION_PDF "application/pdf"
#define CONTENT_TYPE_APPLICATION_ZIP "application/zip"
#define CONTENT_TYPE_APPLICATION_GZIP "application/gzip"
#define CONTENT_TYPE_APPLICATION_TAR "application/x-tar"
#define CONTENT_TYPE_APPLICATION_RAR "application/vnd.rar"
#define CONTENT_TYPE_APPLICATION_7Z "application/x-7z-compressed"
#define CONTENT_TYPE_APPLICATION_SQL "application/sql"
#define CONTENT_TYPE_APPLICATION_GRAPHQL "application/graphql"

// Image types
#define CONTENT_TYPE_IMAGE_PNG "image/png"
#define CONTENT_TYPE_IMAGE_JPEG "image/jpeg"
#define CONTENT_TYPE_IMAGE_GIF "image/gif"
#define CONTENT_TYPE_IMAGE_WEBP "image/webp"
#define CONTENT_TYPE_IMAGE_SVG_XML "image/svg+xml"
#define CONTENT_TYPE_IMAGE_BMP "image/bmp"
#define CONTENT_TYPE_IMAGE_TIFF "image/tiff"
#define CONTENT_TYPE_IMAGE_ICON "image/x-icon"

// Audio types
#define CONTENT_TYPE_AUDIO_MPEG "audio/mpeg"
#define CONTENT_TYPE_AUDIO_OGG "audio/ogg"
#define CONTENT_TYPE_AUDIO_WAV "audio/wav"
#define CONTENT_TYPE_AUDIO_WEBM "audio/webm"
#define CONTENT_TYPE_AUDIO_AAC "audio/aac"
#define CONTENT_TYPE_AUDIO_FLAC "audio/flac"

// Video types
#define CONTENT_TYPE_VIDEO_MP4 "video/mp4"
#define CONTENT_TYPE_VIDEO_MPEG "video/mpeg"
#define CONTENT_TYPE_VIDEO_WEBM "video/webm"
#define CONTENT_TYPE_VIDEO_OGG "video/ogg"
#define CONTENT_TYPE_VIDEO_X_MSVIDEO "video/x-msvideo"
#define CONTENT_TYPE_VIDEO_X_FLV "video/x-flv"

// Multipart types
#define CONTENT_TYPE_MULTIPART_FORM_DATA "multipart/form-data"
#define CONTENT_TYPE_MULTIPART_BYTERANGES "multipart/byteranges"

// Methods
#define METHOD_GET "GET"
#define METHOD_POST "POST"
#define METHOD_HEAD "HEAD"
#define METHOD_DELETE "DELETE"

typedef enum {
	HTTP_ERROR_NULL = 0,
	HTTP_ERROR_PARSING_STATUS_LINE,
	HTTP_ERROR_PARSING_HEADERS,
	HTTP_ERROR_MAKING_REQUEST,
} HTTP_Error;

// String

char *http_strdup(const char *str);
uint8_t *http_read_file(const char *path, size_t *out_size);

// String builder

typedef struct {
	char   *str;
	size_t	cnt;
	size_t	cap;
} HTTP_StringBuilder;

HTTP_StringBuilder http_sb_create(size_t cap);
void http_sb_ensure_capacity(HTTP_StringBuilder *sb, size_t extra);
void http_sb_append_str(HTTP_StringBuilder *sb, const char *s);
void http_sb_append_strf(HTTP_StringBuilder *sb, const char *fmt, ...);
void http_sb_append_char(HTTP_StringBuilder *sb, char ch);
void http_sb_reset(HTTP_StringBuilder *sb);
void http_sb_destroy(HTTP_StringBuilder *sb);
char *http_sb_to_str(HTTP_StringBuilder sb);

// HTTP

typedef struct {
	char *key;
	char *value;
} HTTP_Header;

typedef struct {
	HTTP_Header *headers;
	size_t count;
	size_t capacity;
} HTTP_Headers;

HTTP_Headers http_headers_create(size_t cap);
void http_headers_add(HTTP_Headers *hh, HTTP_Header header);
char *http_headers_get(HTTP_Headers *hh, const char *key);
void http_headers_destroy(HTTP_Headers *hh);

typedef struct {
	char *method;
	char *target;
	char *protocol;
	HTTP_Headers headers;
	uint8_t *body;
	size_t body_len;
} HTTP_Request;

#define http_req_ensure_method(req, resp, mt) \
	while (strcmp((req)->method, (mt)) != 0) { \
		http_resp_set_status_line((resp), STATUS_METHOD_NOT_ALLOWED, ""); \
		return; \
	}

HTTP_Request http_req_create();
HTTP_Request http_req_parse(uint8_t *bytes, HTTP_Error *err);
void http_req_add_header(HTTP_Request *hr, const char *key, const char *value);
void http_req_set_status_line(HTTP_Request *hr, const char *method, const char *target);
void http_req_set_body(HTTP_Request *hr, uint8_t *body, size_t len);
void http_req_destroy(HTTP_Request *hr);
char *http_req_header_to_str(HTTP_Request *hr);

typedef struct {
	char *protocol;
	uint16_t status_code;
	char *reason_phrase;
	HTTP_Headers headers;
	uint8_t *body;
	size_t body_len;
} HTTP_Response;

HTTP_Response http_resp_create();
HTTP_Response http_resp_parse(uint8_t *bytes, HTTP_Error *err);
void http_resp_add_header(HTTP_Response *hr, const char *key, const char *value);
void http_resp_set_status_line(HTTP_Response *hr, uint16_t status_code, const char *reason_phrase);
void http_resp_set_body(HTTP_Response *hr, uint8_t *body, size_t len);
void http_resp_destroy(HTTP_Response *hr);
char *http_resp_header_to_str(HTTP_Response *hr);
HTTP_Response http_make_request(HTTP_Request *req, const char *host, uint16_t port, HTTP_Error *err);

typedef void (*HTTP_HandleFunc)(void *ctx, HTTP_Request *req, HTTP_Response *resp);

typedef struct {
	int socket;
	struct sockaddr_in addr;
	const char **targets;
	HTTP_HandleFunc *hfs;
	void **hfs_ctx;
	size_t hfs_count;
	size_t hfs_cap;
} HTTP_Server;

HTTP_Server http_server_create(uint16_t port);
void http_server_run(HTTP_Server *serv);
void http_server_handle(HTTP_Server *serv, const char *target, HTTP_HandleFunc hf, void *ctx);
int http_server_serve_file(HTTP_Server *serv, const char *target, const char *content_type, const char *path);

#endif // HTTP_H

#ifdef HTTP_IMPLEMENTATION

// String

char *http_strdup(const char *str) {
	size_t str_len = strlen(str) + 1;
	char *mstr = (char *) malloc(sizeof(char) * str_len);
	memcpy(mstr, str, str_len);
	return mstr;
}

uint8_t *read_file(const char *path, size_t *out_size) {
	FILE *f = fopen(path, "rb");
	if (!f) return NULL;
	if (fseek(f, 0, SEEK_END) != 0) { fclose(f); return NULL; }
	long sz = ftell(f);
	if (sz < 0) { fclose(f); return NULL; }
	rewind(f);
	uint8_t *buf = (uint8_t *) malloc((size_t)sz + 1);
	if (!buf) { fclose(f); return NULL; }
	size_t r = fread(buf, 1, (size_t)sz, f);
	fclose(f);
	if (out_size) *out_size = r;
	return buf;
}

// HTTP headers

HTTP_Headers http_headers_create(size_t cap) {
	if (cap == 0) cap = 16;

	return (HTTP_Headers) {
		.headers = (HTTP_Header *) malloc(sizeof(HTTP_Header) * cap),
			.count = 0,
			.capacity = cap,
	};
}

void http_headers_add(HTTP_Headers *hh, HTTP_Header header) {
	if (hh->count == hh->capacity) {
		hh->capacity *= 2;
		hh->headers = (HTTP_Header *) realloc(hh->headers, sizeof(HTTP_Header) * hh->capacity);
	}

	hh->headers[hh->count++] = header;
}

char *http_headers_get(HTTP_Headers *hh, const char *key) {
	for (size_t i = 0; i < hh->count; i++) {
		if (strcmp(hh->headers[i].key, key) == 0)
			return hh->headers[i].value;
	}

	return NULL;
}

void http_headers_destroy(HTTP_Headers *hh) {
	for (size_t i = 0; i < hh->count; i++) {
		free(hh->headers[i].key);
		free(hh->headers[i].value);
	}

	free(hh->headers);
	hh->headers = NULL;
	hh->count = 0;
	hh->headers = 0;
}

// HTTP Request

static ssize_t send_all(int sock, const void *buf, size_t len) {
	size_t sent = 0;
	const uint8_t *b = (const uint8_t*) buf;
	while (sent < len) {
		ssize_t s = send(sock, b + sent, len - sent, 0);
		if (s <= 0) return -1;
		sent += (size_t)s;
	}
	return (ssize_t)sent;
}

HTTP_Response http_make_request(HTTP_Request *req, const char *host, uint16_t port, HTTP_Error *err) {
	*err = HTTP_ERROR_NULL;
	char *header = http_req_header_to_str(req);
	if (!header) return (HTTP_Response){0};
	size_t header_len = strlen(header);

	size_t req_len = header_len + req->body_len;
	if (req_len > 8 * 1024) { fprintf(stderr, "request too large\n"); return (HTTP_Response){0}; }

	uint8_t request[8 * 1024];
	memcpy(request, header, header_len);
	if (req->body_len > 0 && req->body != NULL)
		memcpy(request + header_len, req->body, req->body_len);

	struct hostent *h = gethostbyname(host);
	if (!h) { *err = HTTP_ERROR_MAKING_REQUEST; return (HTTP_Response){0}; }

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) { *err = HTTP_ERROR_MAKING_REQUEST; return (HTTP_Response){0}; }

	struct sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if (h->h_addrtype == AF_INET) {
		memcpy(&addr.sin_addr, h->h_addr_list[0], sizeof(struct in_addr));
	} else {
		close(sock);
		*err = HTTP_ERROR_MAKING_REQUEST;
		return (HTTP_Response){0};
	}

	if (connect(sock, (struct sockaddr *)&addr, sizeof addr) < 0) {
		close(sock);
		*err = HTTP_ERROR_MAKING_REQUEST;
		return (HTTP_Response){0};
	}

	if (send_all(sock, request, req_len) != (ssize_t)req_len) {
		close(sock);
		*err = HTTP_ERROR_MAKING_REQUEST;
		return (HTTP_Response){0};
	}

	uint8_t *resp_buf = (uint8_t *) malloc(1024 * 1024);
	if (!resp_buf) {
		close(sock);
		free(resp_buf);
		*err = HTTP_ERROR_MAKING_REQUEST;
		return (HTTP_Response){0};
	}

	uint8_t buf[4096];
	ssize_t n;
	size_t ln = 0;
	while ((n = recv(sock, buf, sizeof buf, 0)) > 0) {
		if (ln + (size_t)n >= 1024 * 1024) break;
		memcpy(resp_buf + ln, buf, (size_t)n);
		ln += (size_t)n;
	}

	if (n < 0) {
		close(sock);
		free(resp_buf);
		*err = HTTP_ERROR_MAKING_REQUEST;
		return (HTTP_Response){0};
	} 

	if (ln < 1024 * 1024) resp_buf[ln] = '\0'; else resp_buf[1024*1024-1] = '\0';

	HTTP_Error resp_err;
	HTTP_Response resp = http_resp_parse(resp_buf, &resp_err);
	if (resp_err) {
		free(resp_buf);
		close(sock);
		*err = resp_err;
	}

	free(resp_buf);
	close(sock);
	return resp;
}

HTTP_Request http_req_create() {
	return (HTTP_Request) {NULL, 0, NULL, http_headers_create(0)};
}

HTTP_Request http_req_parse(uint8_t *bytes, HTTP_Error *err) {
	HTTP_Request req = http_req_create();
	*err = HTTP_ERROR_NULL;

	char *str = (char *) bytes;
	char *lp = str;
	size_t sl_cnt = 0;

	// status line parsing
	while (!(str[-1] == '\r' && *str == '\n')) {
		if (*str == '\n') {
			*err = HTTP_ERROR_PARSING_STATUS_LINE;
			return req;
		} else if (*str == ' ' || (*str == '\r' && str[1] == '\n')) {
			char tmp = *str;
			*str = '\0';
			switch (sl_cnt++) {
				case 0: req.method = http_strdup(lp); break;
				case 1: req.target = http_strdup(lp); break;
				case 2: req.protocol = http_strdup(lp); break;
				default: break;
			}
			*str = tmp;
			lp = str + 1;
		}
		str++;
	}
	str++;

	if (req.method == NULL || req.target == NULL || req.protocol == NULL) {
		*err = HTTP_ERROR_PARSING_STATUS_LINE;
		return req;
	}

	// headers parsing
	while (!(str[-2] == '\r' && str[-1] == '\n' && str[0] == '\r' && str[1] == '\n')) {
		HTTP_Header header = {0};
		const char *lp = str;

		while (*str && !(*str == ':' && *(str+1) == ' ')) str++;
		if (!*str) {
			*err = HTTP_ERROR_PARSING_HEADERS;
			return req;
		}

		header.key = (char *) malloc(str - lp + 1);
		memcpy(header.key, lp, str - lp);
		header.key[str - lp] = '\0';

		str += 2;
		lp = str;

		while (*str && !(str[0] == '\r' && str[1] == '\n')) str++;
		if (!*str) {
			*err = HTTP_ERROR_PARSING_HEADERS;
			return req;
		}

		header.value = (char *) malloc(str - lp + 1);
		memcpy(header.value, lp, str - lp);
		header.value[str - lp] = '\0';

		str += 2;

		http_headers_add(&req.headers, header);
	}

	str += 2;

	char *content_length = http_headers_get(&req.headers, "Content-Length");
	if (content_length == NULL) {
		return req;
	}

	req.body_len = atoll(content_length);
	if (req.body_len == 0) {
		*err = HTTP_ERROR_PARSING_HEADERS;
		return req;
	}

	req.body = (uint8_t *) malloc(req.body_len * sizeof(uint8_t));
	memcpy(req.body, str, req.body_len * sizeof(uint8_t));

	return req;
}

void http_req_add_header(HTTP_Request *hr, const char *key, const char *value) {
	http_headers_add(&hr->headers, (HTTP_Header) {http_strdup(key), http_strdup(value)});
}

void http_req_set_status_line(HTTP_Request *hr, const char *method, const char *target) {
	hr->method = http_strdup(method);
	hr->target = http_strdup(target);
	hr->protocol = http_strdup(PROTOCOL);
}

void http_req_set_body(HTTP_Request *hr, uint8_t *body, size_t len) {
	char buf[32];
	sprintf(buf, "%zu", len);
	http_req_add_header(hr, "Content-Length", buf);
	hr->body = body;
	hr->body_len = len;
}

char *http_req_header_to_str(HTTP_Request *hr) {
	HTTP_StringBuilder str = http_sb_create(128);
	http_sb_append_strf(&str, "%s %s %s\r\n", hr->method, hr->target, hr->protocol);

	for (size_t i = 0; i < hr->headers.count; i++) {
		http_sb_append_strf(
				&str, "%s: %s\r\n",
				hr->headers.headers[i].key,
				hr->headers.headers[i].value);
	}

	http_sb_append_str(&str, "\r\n");
	return http_sb_to_str(str);
}

void http_req_destroy(HTTP_Request *hr) {
	free(hr->method);
	free(hr->protocol);
	free(hr->target);
	http_headers_destroy(&hr->headers);
	free(hr->body);
}

// HTTP Response

HTTP_Response http_resp_create() {
	return (HTTP_Response) {http_strdup(PROTOCOL), 0, NULL, http_headers_create(0)};
}

HTTP_Response http_resp_parse(uint8_t *bytes, HTTP_Error *err) {
	HTTP_Response resp = http_resp_create();
	*err = HTTP_ERROR_NULL;

	char *str = (char *) bytes;
	char *lp = str;
	size_t sl_cnt = 0;

	// status line parsing
	while (!(str[-1] == '\r' && *str == '\n')) {
		if (*str == '\n') {
			*err = HTTP_ERROR_PARSING_STATUS_LINE;
			return resp;
		} else if (*str == ' ' || (*str == '\r' && str[1] == '\n')) {
			char tmp = *str;
			*str = '\0';
			switch (sl_cnt++) {
				case 0: resp.protocol = http_strdup(lp); break;
				case 1: resp.status_code = (uint16_t) atoi(lp); break;
				case 2: resp.reason_phrase = http_strdup(lp); break;
				default: break;
			}
			*str = tmp;
			lp = str + 1;
		}
		str++;
	}
	str++;

	if (resp.status_code == 0 || resp.reason_phrase == NULL || resp.protocol == NULL) {
		*err = HTTP_ERROR_PARSING_STATUS_LINE;
		return resp;
	}

	// headers parsing
	while (!(str[-2] == '\r' && str[-1] == '\n' && str[0] == '\r' && str[1] == '\n')) {
		HTTP_Header header = {0};
		const char *lp = str;

		while (*str && !(*str == ':' && *(str+1) == ' ')) str++;
		if (!*str) {
			*err = HTTP_ERROR_PARSING_HEADERS;
			return resp;
		}

		header.key = (char *) malloc(str - lp + 1);
		memcpy(header.key, lp, str - lp);
		header.key[str - lp] = '\0';

		str += 2;
		lp = str;

		while (*str && !(str[0] == '\r' && str[1] == '\n')) str++;
		if (!*str) {
			*err = HTTP_ERROR_PARSING_HEADERS;
			return resp;
		}

		header.value = (char *) malloc(str - lp + 1);
		memcpy(header.value, lp, str - lp);
		header.value[str - lp] = '\0';

		str += 2;

		http_headers_add(&resp.headers, header);
	}

	str += 2;

	char *content_length = http_headers_get(&resp.headers, "Content-Length");
	if (content_length == NULL) {
		return resp;
	}

	resp.body_len = atoll(content_length);
	if (resp.body_len == 0) {
		*err = HTTP_ERROR_PARSING_HEADERS;
		return resp;
	}

	resp.body = (uint8_t *) malloc(resp.body_len * sizeof(uint8_t));
	memcpy(resp.body, (uint8_t *) str, resp.body_len * sizeof(uint8_t));

	return resp;
}

void http_resp_set_status_line(HTTP_Response *hr, uint16_t status_code, const char *reason_phrase) {
	hr->status_code = status_code;
	hr->reason_phrase = http_strdup(reason_phrase);
}

void http_resp_set_body(HTTP_Response *hr, uint8_t *body, size_t len) {
	char buf[128];
	sprintf(buf, "%zu", len);
	http_resp_add_header(hr, "Content-Length", buf);
	hr->body = body;
	hr->body_len = len;
}

void http_resp_add_header(HTTP_Response *hr, const char *key, const char *value) {
	http_headers_add(&hr->headers, (HTTP_Header) {http_strdup(key), http_strdup(value)});
}

char *http_resp_header_to_str(HTTP_Response *hr) {
	HTTP_StringBuilder str = http_sb_create(128);
	http_sb_append_strf(&str, "%s %i %s\r\n", hr->protocol, (int)hr->status_code, hr->reason_phrase);

	for (size_t i = 0; i < hr->headers.count; i++) {
		http_sb_append_strf(
				&str, "%s: %s\r\n",
				hr->headers.headers[i].key,
				hr->headers.headers[i].value);
	}

	http_sb_append_str(&str, "\r\n");
	return http_sb_to_str(str);
}

void http_resp_destroy(HTTP_Response *hr) {
	free(hr->reason_phrase);
	free(hr->protocol);
	http_headers_destroy(&hr->headers);
	free(hr->body);
}

static void http_server_grow_if_needed(HTTP_Server *serv) {
	if (serv->hfs_count < serv->hfs_cap) return;
	size_t newcap = serv->hfs_cap ? serv->hfs_cap * 2 : 8;

	const char **nt = (const char **) realloc((void*)serv->targets, sizeof(*nt) * newcap);
	if (!nt) { perror("realloc targets"); exit(1); }
	serv->targets = nt;

	HTTP_HandleFunc *nh = (HTTP_HandleFunc *) realloc(serv->hfs, sizeof(*nh) * newcap);
	if (!nh) { perror("realloc hfs"); exit(1); }
	serv->hfs = nh;

	void **nc = (void **) realloc(serv->hfs_ctx, sizeof(*nc) * newcap);
	if (!nc) { perror("realloc hfs_ctx"); exit(1); }
	serv->hfs_ctx = nc;

	serv->hfs_cap = newcap;
}

void http_server_handle(HTTP_Server *serv, const char *target, HTTP_HandleFunc hf, void *ctx) {
	http_server_grow_if_needed(serv);
	serv->targets[serv->hfs_count] = target;
	serv->hfs[serv->hfs_count] = hf;
	serv->hfs_ctx[serv->hfs_count] = ctx;
	serv->hfs_count++;
}

HTTP_Server http_server_create(uint16_t port) {
	HTTP_Server serv = {0};

	serv.hfs_cap = 32;
	serv.targets = (const char **) malloc(sizeof(char*) * serv.hfs_cap);
	serv.hfs = (HTTP_HandleFunc *) malloc(sizeof(HTTP_HandleFunc) * serv.hfs_cap);
	serv.hfs_ctx = (void **) malloc(sizeof(void*) * serv.hfs_cap);
	if (!serv.targets || !serv.hfs || !serv.hfs_ctx) {
		perror("malloc");
		exit(1);
	}

	serv.socket = socket(AF_INET, SOCK_STREAM, 0);
	if (serv.socket < 0) { perror("socket"); exit(1); }

	int yes = 1;
	if (setsockopt(serv.socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
		perror("setsockopt");
	}

	serv.addr.sin_family = AF_INET;
	serv.addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv.addr.sin_port = htons(port);

	return serv;
}

void http_server_run(HTTP_Server *serv) {
	if (bind(serv->socket, (struct sockaddr*)&serv->addr, sizeof(serv->addr)) < 0) {
		perror("bind"); exit(1);
	}

	if (listen(serv->socket, 16) < 0) {
		perror("listen"); exit(1);
	}

	for (;;) {
		int c = accept(serv->socket, NULL, NULL);
		if (c < 0) {
			if (errno == EINTR) continue;
			perror("accept"); break;
		}

		uint8_t req_buf[1024 * 16];
		ssize_t rn = recv(c, req_buf, sizeof(req_buf) - 1, 0);
		if (rn <= 0) {
			close(c);
			continue;
		}

		HTTP_Error err;
		HTTP_Response resp = http_resp_create();
		HTTP_Request req = http_req_parse(req_buf, &err);

		if (err) {
			http_resp_set_status_line(&resp, STATUS_BAD_REQUEST, "Bad Request");
			http_resp_add_header(&resp, "Connection", "close");
			char *resp_str = http_resp_header_to_str(&resp);
			write(c, resp_str, strlen(resp_str));
		} else {
			bool handled = false;
			for (size_t i = 0; i < serv->hfs_count; i++) {
				const char *t = serv->targets[i];
				size_t tlen = strlen(t);
				if (strncmp(t, req.target, tlen) == 0) {
					if (tlen == 1 && strlen(req.target) != 1) continue;

					serv->hfs[i](serv->hfs_ctx[i], &req, &resp);

					char *resp_str = http_resp_header_to_str(&resp);
					write(c, resp_str, strlen(resp_str));

					write(c, resp.body, resp.body_len);
					handled = true;
					break;
				}
			}
			if (!handled) {
				http_resp_set_status_line(&resp, STATUS_NOT_FOUND, "Not Found");
				http_resp_add_header(&resp, "Connection", "close");

				char *not_found_msg = http_strdup("404 Not Found");
				http_resp_set_body(&resp, (uint8_t *) not_found_msg, strlen(not_found_msg));

				char *resp_str = http_resp_header_to_str(&resp);
				write(c, resp_str, strlen(resp_str));
				write(c, resp.body, resp.body_len);
			}
		}

		http_resp_destroy(&resp);
		http_req_destroy(&req);
		close(c);
	}
}

typedef struct {
	char *content_type;
	char *path;
} ServeFileCtx;

void *serve_file_ctx_create(const char *content_type, const char *file) {
	ServeFileCtx *ctx = (ServeFileCtx *) malloc(sizeof *ctx);
	if (!ctx) return NULL;
	ctx->content_type = http_strdup(content_type);
	ctx->path = http_strdup(file);
	if (!ctx->content_type || !ctx->path) {
		free(ctx->content_type);
		free(ctx->path);
		free(ctx);
		return NULL;
	}
	return ctx;
}

void serve_file_ctx_destroy(void *v) {
	if (!v) return;
	ServeFileCtx *ctx = (ServeFileCtx *) v;
	free(ctx->content_type);
	free(ctx->path);
	free(ctx);
}

void serve_file_handler(void *vctx, HTTP_Request *req, HTTP_Response *resp) {
	ServeFileCtx *ctx = (ServeFileCtx *) vctx;
	if (!ctx) {
		http_resp_set_status_line(resp, STATUS_INTERNAL_SERVER_ERROR, "");
		return;
	}

	http_req_ensure_method(req, resp, METHOD_GET);

	size_t fsize;
	uint8_t *buf = read_file(ctx->path, &fsize);
	if (!buf) {
		http_resp_set_status_line(resp, STATUS_NOT_FOUND, "Not Found");
		http_resp_add_header(resp, "Connection", "close");

		char *not_found_msg = http_strdup("404 Not Found");
		http_resp_set_body(resp, (uint8_t *) not_found_msg, strlen(not_found_msg));
		free(not_found_msg);
		return;
	}

	http_resp_set_status_line(resp, STATUS_OK, "OK");
	http_resp_add_header(resp, "Content-Type", ctx->content_type);

	char lenbuf[32];
	snprintf(lenbuf, sizeof(lenbuf), "%zu", fsize);
	http_resp_add_header(resp, "Content-Length", lenbuf);
	http_resp_add_header(resp, "Connection", "close");

	http_resp_set_body(resp, buf, fsize);
}

int http_server_serve_file(HTTP_Server *serv, const char *target, const char *content_type, const char *path) {
	void *ctx = serve_file_ctx_create(content_type, path);
	if (!ctx) return -1;
	http_server_handle(serv, target, serve_file_handler, ctx);
	return 0;
}

// SB_IMPLEMENTATION

HTTP_StringBuilder http_sb_create(size_t cap) {
	HTTP_StringBuilder sb;
	if (cap == 0) cap = 32;
	sb.str = (char *) malloc(cap);
	sb.cnt = 0;
	sb.cap = cap;
	if (sb.str) sb.str[0] = '\0';
	return sb;
}

void http_sb_ensure_capacity(HTTP_StringBuilder *sb, size_t extra) {
	size_t required = sb->cnt + extra + 1;
	if (required <= sb->cap) return;
	while (sb->cap < required) {
		sb->cap *= 2;
	}
	sb->str = (char *) realloc(sb->str, sb->cap);
}

void http_sb_append_str(HTTP_StringBuilder *sb, const char *s) {
	size_t len = strlen(s);
	http_sb_ensure_capacity(sb, len);
	memcpy(sb->str + sb->cnt, s, len);
	sb->cnt += len;
	sb->str[sb->cnt] = '\0';
}

void http_sb_append_strf(HTTP_StringBuilder *sb, const char *fmt, ...) {
	va_list args, args_copy;
	va_start(args, fmt);

	va_copy(args_copy, args);

	int len = vsnprintf(NULL, 0, fmt, args);
	va_end(args);

	if (len < 0) {
		va_end(args_copy);
		return;
	}

	http_sb_ensure_capacity(sb, (size_t)len);

	vsnprintf(sb->str + sb->cnt, sb->cap - sb->cnt, fmt, args_copy);
	sb->cnt += (size_t)len;

	va_end(args_copy);
}

void http_sb_append_char(HTTP_StringBuilder *sb, char ch) {
	http_sb_ensure_capacity(sb, 1);
	sb->str[sb->cnt++] = ch;
	sb->str[sb->cnt] = '\0';
}

void http_sb_reset(HTTP_StringBuilder *sb) {
	sb->cnt = 0;
	if (sb->str) sb->str[0] = '\0';
}

char *http_sb_to_str(HTTP_StringBuilder sb) {
	return sb.str;
}

void http_sb_destroy(HTTP_StringBuilder *sb) {
	free(sb->str);
	sb->str = NULL;
	sb->cnt = sb->cap = 0;
}

#endif // HTTP_IMPLEMENTATION
