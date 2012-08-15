#include <gtest/gtest.h>
#include <iostream>
#include "dns.h"
#include "utils.h"

using namespace Tins;


class DNSTest : public testing::Test {
public:
    static const uint8_t expected_packet[];
    
    void test_equals(const DNS &dns1, const DNS &dns2);
    void test_equals(const DNS::Query &q1, const DNS::Query &q2);
    void test_equals(const DNS::Resource &q1, const DNS::Resource &q2);
};

const uint8_t DNSTest::expected_packet[] = {
    '\x00', '\x13', '\xd7', '\x9a', '\x00', '\x01', '\x00', '\x01', '\x00', '\x00', '\x00', '\x00', '\x03', 'w', 'w', 'w', '\x07', 'e', 'x', 'a', 'm', 'p', 'l', 'e', '\x03', 'c', 'o', 'm', '\x00', '\x00', '\x01', '\x00', '\x01', '\x03', 'w', 'w', 'w', '\x07', 'e', 'x', 'a', 'm', 'p', 'l', 'e', '\x03', 'c', 'o', 'm', '\x00', '\x00', '\x01', '\x00', '\x01', '\x00', '\x00', '\x12', '4', '\x00', '\x04', '\xc0', '\xa8', '\x00', '\x01'
};



void DNSTest::test_equals(const DNS &dns1, const DNS &dns2) {
    EXPECT_EQ(dns1.id(), dns2.id());
    EXPECT_EQ(dns1.type(), dns2.type());
    EXPECT_EQ(dns1.opcode(), dns2.opcode());
    EXPECT_EQ(dns1.authoritative_answer(), dns2.authoritative_answer());
    EXPECT_EQ(dns1.truncated(), dns2.truncated());
    EXPECT_EQ(dns1.recursion_desired(), dns2.recursion_desired());
    EXPECT_EQ(dns1.recursion_available(), dns2.recursion_available());
    EXPECT_EQ(dns1.z(), dns2.z());
    EXPECT_EQ(dns1.authenticated_data(), dns2.authenticated_data());
    EXPECT_EQ(dns1.checking_disabled(), dns2.checking_disabled());
    EXPECT_EQ(dns1.rcode(), dns2.rcode());
    EXPECT_EQ(dns1.questions(), dns2.questions());
    EXPECT_EQ(dns1.answers(), dns2.answers());
    EXPECT_EQ(dns1.authority(), dns2.authority());
    EXPECT_EQ(dns1.additional(), dns2.additional());
    EXPECT_EQ(dns1.pdu_type(), dns2.pdu_type());
    EXPECT_EQ(dns1.header_size(), dns2.header_size());
    EXPECT_EQ(bool(dns1.inner_pdu()), bool(dns2.inner_pdu()));
}

void DNSTest::test_equals(const DNS::Query &q1, const DNS::Query &q2) {
    EXPECT_EQ(q1.name, q2.name);
    EXPECT_EQ(q1.type, q2.type);
    EXPECT_EQ(q1.qclass, q2.qclass);
}

void DNSTest::test_equals(const DNS::Resource &q1, const DNS::Resource &q2) {
    EXPECT_EQ(q1.dname, q2.dname);
    EXPECT_EQ(q1.addr, q2.addr);
    EXPECT_EQ(q1.type, q2.type);
    EXPECT_EQ(q1.qclass, q2.qclass);
    EXPECT_EQ(q1.ttl, q2.ttl);
}

TEST_F(DNSTest, ConstructorFromBuffer) {
    DNS dns(expected_packet, sizeof(expected_packet));
    // id=0x13, qr=1, opcode=0xa, aa=1, tc=1, rd=1, ra=1, z=0, rcode=0xa
    EXPECT_EQ(dns.id(), 0x13);
    EXPECT_EQ(dns.type(), DNS::RESPONSE);
    EXPECT_EQ(dns.opcode(), 0xa);
    EXPECT_EQ(dns.authoritative_answer(), 1);
    EXPECT_EQ(dns.truncated(), 1);
    EXPECT_EQ(dns.recursion_desired(), 1);
    EXPECT_EQ(dns.recursion_available(), 1);
    EXPECT_EQ(dns.z(), 0);
    EXPECT_EQ(dns.rcode(), 0xa);
    EXPECT_EQ(dns.questions(), 1);
    EXPECT_EQ(dns.answers(), 1);
    
    std::list<DNS::Query> queries = dns.dns_queries();
    ASSERT_EQ(queries.size(), 1);
    test_equals(queries.front(), DNS::Query("www.example.com", DNS::A, DNS::IN));
    
    std::list<DNS::Resource> answers = dns.dns_answers();
    ASSERT_EQ(answers.size(), 1);
    test_equals(answers.front(), DNS::Resource("www.example.com", "192.168.0.1", DNS::A, DNS::IN, 0x1234));
}

TEST_F(DNSTest, CopyConstructor) {
    DNS dns1(expected_packet, sizeof(expected_packet));
    DNS dns2(dns1);
    test_equals(dns1, dns2);
}

TEST_F(DNSTest, CopyAssignmentOperator) {
    DNS dns1(expected_packet, sizeof(expected_packet));
    DNS dns2;
    dns2 = dns1;
    test_equals(dns1, dns2);
}

TEST_F(DNSTest, NestedCopy) {
    DNS *nested = new DNS(expected_packet, sizeof(expected_packet));
    DNS dns1(expected_packet, sizeof(expected_packet));
    dns1.inner_pdu(nested);
    DNS dns2(dns1);
    test_equals(dns1, dns2);
    dns2.inner_pdu(0);
    dns2 = dns1;
    test_equals(dns1, dns2);
}

TEST_F(DNSTest, ID) {
    DNS dns;
    dns.id(0x7263);
    EXPECT_EQ(dns.id(), 0x7263);
}

TEST_F(DNSTest, Type) {
    DNS dns;
    dns.type(DNS::RESPONSE);
    EXPECT_EQ(dns.type(), DNS::RESPONSE);
}

TEST_F(DNSTest, Opcode) {
    DNS dns;
    dns.opcode(0xa);
    EXPECT_EQ(dns.opcode(), 0xa);
}

TEST_F(DNSTest, AuthoritativeAnswer) {
    DNS dns;
    dns.authoritative_answer(1);
    EXPECT_EQ(dns.authoritative_answer(), 1);
}

TEST_F(DNSTest, Truncated) {
    DNS dns;
    dns.truncated(1);
    EXPECT_EQ(dns.truncated(), 1);
}

TEST_F(DNSTest, RecursionDesired) {
    DNS dns;
    dns.recursion_desired(1);
    EXPECT_EQ(dns.recursion_desired(), 1);
}

TEST_F(DNSTest, RecursionAvailable) {
    DNS dns;
    dns.recursion_available(1);
    EXPECT_EQ(dns.recursion_available(), 1);
}

TEST_F(DNSTest, Z) {
    DNS dns;
    dns.z(1);
    EXPECT_EQ(dns.z(), 1);
}

TEST_F(DNSTest, AuthenticatedData) {
    DNS dns;
    dns.authenticated_data(1);
    EXPECT_EQ(dns.authenticated_data(), 1);
}

TEST_F(DNSTest, CheckingDisabled) {
    DNS dns;
    dns.checking_disabled(1);
    EXPECT_EQ(dns.checking_disabled(), 1);
}

TEST_F(DNSTest, RCode) {
    DNS dns;
    dns.rcode(0xa);
    EXPECT_EQ(dns.rcode(), 0xa);
}

TEST_F(DNSTest, Question) {
    DNS dns;
    dns.add_query("www.example.com", DNS::A, DNS::IN);
    dns.add_query("www.example2.com", DNS::MX, DNS::IN);
    EXPECT_EQ(dns.questions(), 2);
}

TEST_F(DNSTest, Answers) {
    DNS dns;
    dns.add_answer("www.example.com", DNS::A, DNS::IN, 0x762, Utils::ip_to_int("127.0.0.1"));
    dns.add_answer("www.example2.com", DNS::MX, DNS::IN, 0x762, Utils::ip_to_int("127.0.0.1"));
    EXPECT_EQ(dns.answers(), 2);
}

