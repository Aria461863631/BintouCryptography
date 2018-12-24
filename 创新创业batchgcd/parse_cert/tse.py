import censys.certificates

UID = "78d24978-4024-45ce-97a0-913fa26b4cdc"
SECRET = "Ee0SBxl9XelzvS6C0RHaBsSCPDTgMpvl"

certificates = censys.certificates.CensysCertificates(UID, SECRET)
fields = ["parsed.subject_dn",
          "parsed.fingerprint_sha256", "parsed.fingerprint_sha1"]

for c in certificates.search("validation.nss.valid: true", fields=fields):
    print(c["parsed.subject_dn"])
