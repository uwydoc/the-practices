#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Examples from the ORM tutorial
#

from __future__ import print_function, division

from sqlalchemy import Column, Integer, String, Sequence, ForeignKey
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import relationship, backref

Base = declarative_base()

class User(Base):
    __tablename__ = 'user'

    id = Column(Integer, Sequence('user_id_seq'), primary_key=True)
    name = Column(String(50))
    fullname = Column(String(50))
    password = Column(String(12))

    def __repr__(self):
        return "<User(name='%s', fullname='%s', password='%s')>" % (self.name,
                self.fullname, self.password)

class Address(Base):
    __tablename__ = 'address'

    id = Column(Integer, Sequence('address_id_seq'), primary_key=True)
    email_address = Column(String(50))
    user_id = Column(Integer, ForeignKey('user.id'))

    user = relationship('User', backref=backref('addresses', order_by=id))

    def __repr__(self):
        return "<Address(email_address='%s')>" % self.email_address

if __name__ == '__main__':
    from sqlalchemy import create_engine
    from sqlalchemy.orm import sessionmaker

    engine = create_engine('sqlite:///tutorial.db')
    # Create the tables if not exist
    Base.metadata.create_all(engine)
    # Create a session, do some insert and query
    Session = sessionmaker()
    session = Session(bind=engine)
    ed = User(name='ed', fullname='Ed Jacson', password='edpass')
    session.add(ed)
    session.commit()
    # Query
    for row in session.query(User).all():
        print(row)
