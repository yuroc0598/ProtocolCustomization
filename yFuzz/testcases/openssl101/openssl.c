
/*

Let OpenSSL talk to itself
code CC0 by Hanno Böck

To create self signed cert use:
  openssl req -x509 -newkey rsa:512 -keyout server.key -out server.pem -days 9999 -nodes -subj /CN=a/

*/

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#define MAP_SIZE 1 << 16
typedef uint8_t u8;
void err(){
	ERR_print_errors_fp(stderr);
    exit(-1);
}


void write_packet(int c,unsigned char* buf,int r){
	char fname[256];

	sprintf(fname,"in/p%d/packet",c);
    FILE* f = fopen(fname,"wb");
    fwrite(buf,1,r,f);
	fclose(f);
}

int read_packet(int c,unsigned char* buf){
	char fname[256];
	sprintf(fname,"in/p%d/packet",c);
    FILE* f = fopen(fname,"rb");
    int r = fread(buf,1,4096,f);
	fclose(f);
    return r;
}

int main(int argc, char **argv)
{
	int r;
	int c, step;
	unsigned char buf[4096];
	const char ifi[]="input";
	char* res_shm= NULL;
	u8* shmptr;
	FILE *f;
	SSL_CTX *sctx, *cctx;
	SSL *server, *client;
	BIO *sinbio, *soutbio, *cinbio, *coutbio;
//	int tmpfd = open("/dev/null",0);
    
	/*check syntax*/
	if (argc!=1){
		printf("don't need any argv now\n");
		return -1;
	}

	step = 1; 
#ifdef __AFL_HAVE_MANUAL_CONTROL
		res_shm = getenv("__AFL_SHM_ID");
		if(res_shm){
			int shmid = atoi(res_shm);
			shmptr = shmat(shmid,NULL,0);
		}
#endif

	SSL_library_init();
	SSL_load_error_strings();
	ERR_load_BIO_strings();
	OpenSSL_add_all_algorithms();

	if (!(sctx = SSL_CTX_new(TLSv1_method())))
		err();
    

//	if (!SSL_CTX_use_certificate_file(sctx, "/home/yuroc/workspace/protocol/tools/pro2fuzz/testcases/openssl/server.pem", SSL_FILETYPE_PEM))

if (!SSL_CTX_use_certificate_file(sctx, "server.pem", SSL_FILETYPE_PEM))
		err();


if (!SSL_CTX_use_PrivateKey_file(sctx, "server.key", SSL_FILETYPE_PEM))
//	if (!SSL_CTX_use_PrivateKey_file(sctx, "/home/yuroc/workspace/protocol/tools/pro2fuzz/testcases/openssl/server.key", SSL_FILETYPE_PEM))
		err();

	if (!(server = SSL_new(sctx)))
		err();


	sinbio = BIO_new(BIO_s_mem());
	soutbio = BIO_new(BIO_s_mem());
	SSL_set_bio(server, sinbio, soutbio);
	SSL_set_accept_state(server);

	if (!(cctx = SSL_CTX_new(TLSv1_method())))
		err();
	if (!(client = SSL_new(cctx)))
		err();

	cinbio = BIO_new(BIO_s_mem());
	coutbio = BIO_new(BIO_s_mem());
	SSL_set_bio(client, cinbio, coutbio);
	SSL_set_connect_state(client);


	c = 0;
	do {

		r = SSL_do_handshake(client);
		if (r == -1) {
			if ((SSL_get_error(client, r) != SSL_ERROR_WANT_WRITE)
			    && (SSL_get_error(client, r) !=
				SSL_ERROR_WANT_READ)){
	            ERR_print_errors_fp(stderr);
                break;
            }
		}

		r = BIO_read(coutbio, buf, 4096);
		if (r == -1){
            ERR_print_errors_fp(stderr);
            break;
        }
		c++;
		if (c == step) {

#ifdef __AFL_HAVE_MANUAL_CONTROL
			__AFL_INIT();
#endif

			if(res_shm)
			step = shmptr[MAP_SIZE];
	//		write(tmpfd,"%d",step);
			if(step == c){
		    	f = fopen(ifi, "rb");
	    		r = fread(buf, 1, 4096, f);
		    	fclose(f);
				write_packet(c,buf,r);
			}
			else r = read_packet(c,buf);
		} 
        
        BIO_write(sinbio, buf, r);

		r = SSL_do_handshake(server);
		if (r == -1) {
			if ((SSL_get_error(server, r) != SSL_ERROR_WANT_WRITE)
			    && (SSL_get_error(server, r) !=
				SSL_ERROR_WANT_READ)){
				ERR_print_errors_fp(stderr);
                break;
            }

		}

		r = BIO_read(soutbio, buf, 4096);
		if (r == -1){
		    ERR_print_errors_fp(stderr);
            break;
        }
		c++;
        
		if (c == step) {
#ifdef __AFL_HAVE_MANUAL_CONTROL
			__AFL_INIT();
#endif

			if(res_shm)
			step = shmptr[MAP_SIZE];
	//		write(tmpfd,"%d",step);
			if(step == c){
				f = fopen(ifi, "rb");
				r = fread(buf, 1, 4096, f);
				fclose(f);
				write_packet(c,buf,r);
			}
			else r = read_packet(c,buf);
		} 

        BIO_write(cinbio, buf, r);

		printf("server state: %s / %s\n", SSL_state_string(server),
		       SSL_state_string_long(server));
		printf("client state: %s / %s\n", SSL_state_string(client),
		       SSL_state_string_long(client));
	} while ((!SSL_is_init_finished(server)
		  || !SSL_is_init_finished(client)) && c < 10);
    
#ifdef __AFL_HAVE_MANUAL_CONTROL
	if(res_shm){
	shmptr[(MAP_SIZE) + 1] = c;
	shmdt(shmptr);
	}
#endif
//	close(tmpfd);
	return 0;
}
