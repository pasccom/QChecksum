# Copyright 2019 Pascal COMBES <pascom@orange.fr>
# 
# This file is part of QChecksum.
# 
# QChecksum is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# QChecksum is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with QChecksum. If not, see <http://www.gnu.org/licenses/>

""" This module allows to describe `Freedesktop entries <https://specifications.freedesktop.org/desktop-entry-spec/desktop-entry-spec-latest.html>`_ in Python.
    The ``*.desktop`` files representing the services can be generated from these Python descriptions.
"""

class LocaleString:
    """ Creates a text with translations.

        :param str default: The default text
        :param dict kwArgs: Translations can be added by using the locale codes (e.g. ``fr``) as keywords and the translations as values
    """

    def __init__(self, default, **kwArgs):
        self.__default = default
        self.__localized = kwArgs

    def __setitem__(self, code, text):
        """ Adds a translation

            :param str code: The locale code (e.g. ``fr``)
            :param str text: The translation text
        """
        self.__localized[code] = text

    def default(self, field):
        """ Returns the default text prefixed by the field.

            :param str field: The field prefix
            :rtype: str
            :returns int: The default text prefixed by the field prefix
        """
        return f'{field}={self.__default}'

    def localized(self, field):
        """ Generator yielding translations prefixed by the field.

            :param str field: The field prefix
            :rtype: str
            :yields: Translations prefixed by the field prefix
        """
        yield f'{field}={self.__default}'
        for k, v in self.__localized.items():
            yield f'{field}[{k}]={v}'

class DesktopAction(dict):
    """ Python representation of a `Freedesktop action <https://specifications.freedesktop.org/desktop-entry-spec/desktop-entry-spec-latest.html#extra-actions>`_.

        :param str name: Internal name of the action
        :param LocaleString entryName: Name of the action
        :param kwArgs: Other attributes of the action
    """

    requiredKeys = ['Name']

    def __init__(self, name, entryName, **kwArgs):
        super().__init__()
        self.__name = name
        self.__setitem__('Name', entryName)
        for k, v in kwArgs.items():
            self.__setitem__(k, v)

    def __str__(self):
        """ Returns the internal name of the Freedesktop action.

            :rtype: str
            :returns: The internal name of the action
        """
        return self.__name

    def __repr__(self):
        """ Returns the textual representation of the Freedesktop action.

            :rtype: str
            :returns: The textual representation of the action
        """
        ret = [f'[Desktop Action {self.__name}]']
        for k in DesktopAction.requiredKeys + [k for k in self.keys() if k not in DesktopAction.requiredKeys]:
            if type(self.__getitem__(k)) is LocaleString:
                ret += list(self.__getitem__(k).localized(k))
            else:
                ret += [f'{k}={self.__getitem__(k)}']
        return '\n'.join(ret)

class DesktopEntry(dict):
    """ Python representation of a `Freedesktop entry <https://specifications.freedesktop.org/desktop-entry-spec/desktop-entry-spec-latest.html>`_.

        :param str entryType: Type of the entry
        :param LocaleString entryName: Name of the entry
        :param kwArgs: Other attributes of the entry
    """

    requiredKeys = ['Type', 'Name']

    def __init__(self, entryType, entryName, **kwArgs):
        super().__init__()
        self.__setitem__('Type', entryType)
        self.__setitem__('Name', entryName)
        for k, v in kwArgs.items():
            self.__setitem__(k, v)

    def __repr__(self):
        """ Returns the textual representation of the Freedesktop entry.

            :rtype: str
            :returns: The textual representation of the entry
        """
        ret = ['[Desktop Entry]']

        for k in DesktopEntry.requiredKeys + [k for k in self.keys() if k not in DesktopEntry.requiredKeys]:
            if type(self.__getitem__(k)) is LocaleString:
                ret += list(self.__getitem__(k).localized(k))
            elif type(self.__getitem__(k)) is list:
                ret += [k + '=' + ';'.join([str(item) for item in self.__getitem__(k)]) + ';']
            else:
                ret += [f'{k}={self.__getitem__(k)}']

        for k in self.keys():
            if type(self.__getitem__(k)) is DesktopAction:
                ret += ['', repr(self.__getitem__(k))]
            elif type(self.__getitem__(k)) is list:
                for a in self.__getitem__(k):
                    if type(a) is DesktopAction:
                        ret += ['', repr(a)]

        return '\n'.join(ret) + '\n'

class ServiceEntry(DesktopEntry):
    """ Python representation of a `Freedesktop service entry  <https://specifications.freedesktop.org/desktop-entry-spec/desktop-entry-spec-latest.html>`_.

        :param LocaleString entryName: Name of the service
        :param kwArgs: Other attributes of the service
    """
    def __init__(self, entryName, **kwArgs):
        super().__init__('Service', entryName, **kwArgs)
