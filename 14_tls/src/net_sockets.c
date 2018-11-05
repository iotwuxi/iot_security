#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <kernel.h>
#include <sys/fcntl.h>
#include <net/socket.h>
#include <net/net_app.h>

#include "mbedtls/platform.h"
#include "mbedtls/net_sockets.h"

#define read(fd,buf,len)        recv( fd, (char*)( buf ), (int)( len ), 0 )
#define write(fd,buf,len)       send( fd, (char*)( buf ), (int)( len ), 0 )

/*
 * Prepare for using the sockets interface
 */
static int net_prepare( void )
{
    return( 0 );
}

/*
 * Initialize a context
 */
void mbedtls_net_init( mbedtls_net_context *ctx )
{
    ctx->fd = -1;
}

/*
 * Initiate a TCP connection with host:port and the given protocol
 */
int mbedtls_net_connect( mbedtls_net_context *ctx, const char *host,
                         const char *port, int proto )
{
    int ret;
    struct addrinfo hints, *addr_list, *cur;

    if( ( ret = net_prepare() ) != 0 )
        return( ret );

    /* Do name resolution with both IPv6 and IPv4 */
    memset( &hints, 0, sizeof( hints ) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = proto == MBEDTLS_NET_PROTO_UDP ? SOCK_DGRAM : SOCK_STREAM;
    hints.ai_protocol = proto == MBEDTLS_NET_PROTO_UDP ? IPPROTO_UDP : IPPROTO_TCP;

    if( getaddrinfo( host, port, &hints, &addr_list ) != 0 )
        return( MBEDTLS_ERR_NET_UNKNOWN_HOST );

    /* Try the sockaddrs until a connection succeeds */
    ret = MBEDTLS_ERR_NET_UNKNOWN_HOST;
    for( cur = addr_list; cur != NULL; cur = cur->ai_next )
    {
        ctx->fd = (int) socket( cur->ai_family, cur->ai_socktype,
                            cur->ai_protocol );
        if( ctx->fd < 0 )
        {
            ret = MBEDTLS_ERR_NET_SOCKET_FAILED;
            continue;
        }

        if( connect( ctx->fd, cur->ai_addr, cur->ai_addrlen ) == 0 )
        {
            ret = 0;
            break;
        }

        close( ctx->fd );
        ret = MBEDTLS_ERR_NET_CONNECT_FAILED;
    }

    freeaddrinfo( addr_list );

    return( ret );
}

/*
 * Check if the requested operation would be blocking on a non-blocking socket
 * and thus 'failed' with a negative return value.
 *
 * Note: on a blocking socket this function always returns 0!
 */
static int net_would_block( const mbedtls_net_context *ctx )
{
    int err = errno;

    /*
     * Never return 'WOULD BLOCK' on a non-blocking socket
     */
    if( ( fcntl( ctx->fd, F_GETFL, 0 ) & O_NONBLOCK ) != O_NONBLOCK )
    {
        errno = err;
        return( 0 );
    }

    switch( errno = err )
    {
#if defined EAGAIN
        case EAGAIN:
#endif
#if defined EWOULDBLOCK && EWOULDBLOCK != EAGAIN
        case EWOULDBLOCK:
#endif
            return( 1 );
    }
    return( 0 );
}

/*
 * Set the socket blocking or non-blocking
 */
int mbedtls_net_set_block( mbedtls_net_context *ctx )
{
    return( fcntl( ctx->fd, F_SETFL, fcntl( ctx->fd, F_GETFL, 0 ) & ~O_NONBLOCK ), 0 );
}

int mbedtls_net_set_nonblock( mbedtls_net_context *ctx )
{
    return( fcntl( ctx->fd, F_SETFL, fcntl( ctx->fd, F_GETFL, 0 ) | O_NONBLOCK ), 0 );
}

/*
 * Read at most 'len' characters
 */
int mbedtls_net_recv( void *ctx, unsigned char *buf, size_t len )
{
    int ret;
    int fd = ((mbedtls_net_context *) ctx)->fd;

    if( fd < 0 )
        return( MBEDTLS_ERR_NET_INVALID_CONTEXT );

    ret = (int) read( fd, buf, len );

    if( ret < 0 )
    {
        if( net_would_block( ctx ) != 0 )
            return( MBEDTLS_ERR_SSL_WANT_READ );

        if( errno == EPIPE || errno == ECONNRESET )
            return( MBEDTLS_ERR_NET_CONN_RESET );

        if( errno == EINTR )
            return( MBEDTLS_ERR_SSL_WANT_READ );

        return( MBEDTLS_ERR_NET_RECV_FAILED );
    }

    return( ret );
}

/*
 * Write at most 'len' characters
 */
int mbedtls_net_send( void *ctx, const unsigned char *buf, size_t len )
{
    int ret;
    int fd = ((mbedtls_net_context *) ctx)->fd;

    if( fd < 0 )
        return( MBEDTLS_ERR_NET_INVALID_CONTEXT );

    ret = (int) write( fd, buf, len );

    if( ret < 0 )
    {
        if( net_would_block( ctx ) != 0 )
            return( MBEDTLS_ERR_SSL_WANT_WRITE );

        if( errno == EPIPE || errno == ECONNRESET )
            return( MBEDTLS_ERR_NET_CONN_RESET );

        if( errno == EINTR )
            return( MBEDTLS_ERR_SSL_WANT_WRITE );

        return( MBEDTLS_ERR_NET_SEND_FAILED );
    }

    return( ret );
}

/*
 * Gracefully close the connection
 */
void mbedtls_net_free( mbedtls_net_context *ctx )
{
    if( ctx->fd == -1 )
        return;

#if 0
    shutdown( ctx->fd, 2 );
#endif

    close( ctx->fd );
    ctx->fd = -1;
}

/*
 * Portable usleep helper
 */
void mbedtls_net_usleep( unsigned long usec )
{
    return;
}

/*
 * Create a listening socket on bind_ip:port
 */
int mbedtls_net_bind( mbedtls_net_context *ctx, const char *bind_ip, const char *port, int proto )
{
    return 0;
}

/*
 * Accept a connection from a remote client
 */
int mbedtls_net_accept( mbedtls_net_context *bind_ctx,
                        mbedtls_net_context *client_ctx,
                        void *client_ip, size_t buf_size, size_t *ip_len )
{
    return( 0 );
}

/*
 * Check if data is available on the socket
 */
int mbedtls_net_poll( mbedtls_net_context *ctx, uint32_t rw, uint32_t timeout )
{
    return( 0 );
}

/*
 * Read at most 'len' characters, blocking for at most 'timeout' ms
 */
int mbedtls_net_recv_timeout( void *ctx, unsigned char *buf,
                              size_t len, uint32_t timeout )
{
    return 0;
}
