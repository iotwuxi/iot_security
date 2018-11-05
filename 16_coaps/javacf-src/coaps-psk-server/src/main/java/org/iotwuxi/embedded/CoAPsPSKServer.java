package org.iotwuxi.embedded;

import java.net.InetSocketAddress;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.logging.Level;

import org.eclipse.californium.core.CaliforniumLogger;
import org.eclipse.californium.core.CoapResource;
import org.eclipse.californium.core.CoapServer;
import org.eclipse.californium.core.coap.CoAP.ResponseCode;
import org.eclipse.californium.core.network.CoapEndpoint;
import org.eclipse.californium.core.network.Endpoint;
import org.eclipse.californium.core.network.config.NetworkConfig;
import org.eclipse.californium.core.network.interceptors.MessageTracer;
import org.eclipse.californium.core.server.resources.CoapExchange;
import org.eclipse.californium.scandium.DTLSConnector;
import org.eclipse.californium.scandium.ScandiumLogger;
import org.eclipse.californium.scandium.config.DtlsConnectorConfig;
import org.eclipse.californium.scandium.dtls.cipher.CipherSuite;
import org.eclipse.californium.scandium.dtls.pskstore.InMemoryPskStore;

public class CoAPsPSKServer
{
    static {
        CaliforniumLogger.initialize();
        CaliforniumLogger.setLevel(Level.CONFIG);
        ScandiumLogger.initialize();
        ScandiumLogger.setLevel(Level.FINER);
    }
    
    public static final int DTLS_PORT = 5682;
    public static void main( String[] args )
    {
        CoapServer server = new CoapServer();
        server.add(new CoapResource("secure") {
            @Override
            public void handleGET(CoapExchange exchange) {
                exchange.respond(ResponseCode.CONTENT, "Hello Security!");
            }
        });

        server.add(new CoapResource("time") {
            @Override
            public void handleGET(CoapExchange exchange) {
                Date date = new Date();
                // 打印当前时间
                exchange.respond(ResponseCode.CONTENT,
                        new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(date));
            }
        });

        // Pre-shared secrets
        InMemoryPskStore pskStore = new InMemoryPskStore();
        pskStore.setKey("identity", "password".getBytes());

        DtlsConnectorConfig.Builder config =
                new DtlsConnectorConfig.Builder(new InetSocketAddress(DTLS_PORT));
        config.setSupportedCipherSuites(
                new CipherSuite[]{CipherSuite.TLS_PSK_WITH_AES_128_CCM_8,
                CipherSuite.TLS_PSK_WITH_AES_128_CBC_SHA256});
        config.setPskStore(pskStore);

        DTLSConnector connector = new DTLSConnector(config.build());
        server.addEndpoint(new CoapEndpoint(connector, NetworkConfig.getStandard()));
        server.start();

        // add special interceptor for message traces
        for (Endpoint ep : server.getEndpoints()) {
            ep.addInterceptor(new MessageTracer());
        }

        System.out.println("Secure CoAP PSK Server is listening on port: " + DTLS_PORT);
    }
}
