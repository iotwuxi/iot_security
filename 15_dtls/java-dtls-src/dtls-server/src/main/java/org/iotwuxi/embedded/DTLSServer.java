package org.iotwuxi.embedded;

import org.eclipse.californium.elements.Connector;
import org.eclipse.californium.elements.RawData;
import org.eclipse.californium.elements.RawDataChannel;
import org.eclipse.californium.scandium.DTLSConnector;
import org.eclipse.californium.scandium.config.DtlsConnectorConfig;
import org.eclipse.californium.scandium.dtls.cipher.CipherSuite;
import org.eclipse.californium.scandium.dtls.pskstore.InMemoryPskStore;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.net.InetSocketAddress;


/**
 * @author xukai
 */
public class DTLSServer
{

    private static final int DEFAULT_PORT = 4433;
    private static final Logger LOG = LoggerFactory.getLogger(DTLSServer.class.getName());

    private DTLSConnector dtlsConnector;

    public DTLSServer() {
        InMemoryPskStore pskStore = new InMemoryPskStore();
        byte[] psk = new byte[]{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
        pskStore.setKey("Client_identity", psk);

        DtlsConnectorConfig.Builder builder = new DtlsConnectorConfig.Builder();
        builder.setAddress(new InetSocketAddress(DEFAULT_PORT));
        builder.setPskStore(pskStore);
        builder.setSupportedCipherSuites(CipherSuite.TLS_PSK_WITH_AES_128_CCM_8,
                CipherSuite.TLS_PSK_WITH_AES_128_CBC_SHA256);

        dtlsConnector = new DTLSConnector(builder.build());
        dtlsConnector.setRawDataReceiver(new RawDataChannelImpl(dtlsConnector));
    }

    public void start() {
        try {
            dtlsConnector.start();
        } catch (IOException e) {
            throw new IllegalStateException("Unexpected error starting the DTLS UDP server",e);
        }
    }

    private class RawDataChannelImpl implements RawDataChannel {

        private Connector connector;

        public RawDataChannelImpl(Connector con) {
            this.connector = con;
        }

        @Override
        public void receiveData(final RawData raw) {
            LOG.info("Received request: {}", new String(raw.getBytes()));
            connector.send(RawData.outbound(raw.getBytes(), raw.getEndpointContext(), null, false));
        }
    }

    public static void main(String[] args) {
        DTLSServer server = new DTLSServer();
        server.start();

        try {
            for (;;) {
                Thread.sleep(5000);
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
