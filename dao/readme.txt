- compile environment:
  centos 6.5 x86-64
  postgresql93-devel
    - yum install http://yum.postgresql.org/9.3/redhat/rhel-6-x86_64/pgdg-redhat93-9.3-2.noarch.rpm
    - yum install postgresql93-devel
      below rpm installed:
        - postgresql93-devel-9.3.10-1PGDG.rhel6.x86_64
        - postgresql93-9.3.10-1PGDG.rhel6.x86_64
        - postgresql93-libs-9.3.10-1PGDG.rhel6.x86_64

- modify /etc/profile to add below lines
  export CPLUS_INCLUDE_PATH=/usr/pgsql-9.3/include:$CPLUS_INCLUDE_PATH
  export C_INCLUDE_PATH=/usr/pgsql-9.3/include:$C_INCLUDE_PATH
  export LD_LIBRARY_PATH=/usr/pgsql-9.3/lib:$LD_LIBRARY_PATH
  export LIBRARY_PATH=/usr/pgsql-9.3/lib:$LIBRARY_PATH

  or add above lines in .bashrc (only applicaple for one user)
  or execute above commands in cmd line before 'make'
