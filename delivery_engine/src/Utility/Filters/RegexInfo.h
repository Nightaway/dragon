#ifndef __REGEX_INFO_H__
#define __REGEX_INFO_H__

boost::regex browser_regex("(microsoft internet explorer|msie|netscape6|netscape|galeon|phoenix|mozilla firebird|firebird|firefox|chimera|camino|epiphany|safari|k-meleon|mozilla|opera|konqueror|icab|lynx|links|ncsa mosaic|amaya|omniweb|hotjava|browsex|amigavoyager|amiga-aweb|ibrowse|chrome)");
boost::regex regex_windows( "([wW]in[dows]*)[\\s]?([0-9a-zA-Z]*)[\\w\\s]?([0-9a-zA-Z.]*)");
boost::regex regex_mac("(68[k0]{1,3})|(ppc mac os x)|([p\\S]{1,5}pc)|(darwin)");
boost::regex regex_os2("(os\\/2|ibm-webexplorer/)");
boost::regex regex_sunos( "(sun|i86)[os\\s]*([0-9]*)");
boost::regex regex_irix("(irix)[\\s]*([0-9]*)");
boost::regex regex_hpux( "(hp-ux)[\\s]*([0-9]*)");
boost::regex regex_aix( "(aix([0-9]*))");
boost::regex regex_dec( "(dec|osfl|alphaserver|ultrix|alphastation)");
boost::regex regex_vms("(vax|openvms)");
boost::regex regex_sco( "(sco|unix_sv)");
boost::regex regex_linux("(x11|inux)");
boost::regex regex_bsd("(free)?(bsd)");
boost::regex regex_amiga("(amiga[os]?)");
boost::regex time_regex("(.*)\\-(.*)\\-(.*)\\s(.*)\\:(.*)\\:(.*)");
boost::regex ip_regex("(\\d{1,3}|\\*)\\.(\\d{1,3}|\\*)\\.(\\d{1,3}|\\*)\\.(\\d{1,3}|\\*)");
boost::regex fuction_regex("(\\w+)\\('(.*)', '(.*)'\\)");

#endif