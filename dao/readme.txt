
Development Env setup
---------------------

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


postgresql 9.3 server installation
----------------------------------
# yum install postgresql93-server postgresql93-contrib
  Installing : libxslt-1.1.26-2.el6_3.1.x86_64
  Installing : uuid-1.6.1-10.el6.x86_64
  Installing : postgresql93-contrib-9.3.10-1PGDG.rhel6.x86_64
  Installing : postgresql93-server-9.3.10-1PGDG.rhel6.x86_64 
# service postgresql-9.3 initdb
# chkconfig postgresql-9.3 on

# /etc/init.d/etc/init.d/postgresql-9.3 start

# su - postgres
$ psql
postgres=# ALTER USER postgres WITH PASSWORD 'wapwap12';
postgres=# select * from pg_shadow;

# passwd postgres       -> change the password of linux user 'postgres'

# vi /var/lib/pgsql/9.3/data/pg_hba.conf     -> change as below
    # IPv4 local connections:
    host    all             all             127.0.0.1/32            md5 

# /etc/init.d/etc/init.d/postgresql-9.3 restart

# psql -h 127.0.0.1 -p 5432  -U postgres      -> login test
