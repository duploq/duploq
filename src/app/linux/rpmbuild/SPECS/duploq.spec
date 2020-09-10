Name:		duploq
Version:	0.1
Release:	1
Summary:	GUI frontend for Duplo source code duplicate finder
License:	GPLv3
URL:		https://github/duploq/
BuildRequires:	qt5-devel >= 5.9.0

%description
DuploQ is a GUI frontend for Duplo duplicate finder console tool. Its goal is to find duplicates (i.e. copy-pasted parts of code) across several source files.

Currently DuploQ directly supports C, C++, C#, Java and JavaScript languages, but it can be used for other languages as well.

%install
mkdir -p %{buildroot}/usr/local/bin
cp /usr/local/bin/duplo %buildroot/usr/local/bin
cp /usr/local/bin/duploq %buildroot/usr/local/bin

mkdir -p %{buildroot}/usr/local/share/applications
cp /usr/local/share/applications/duploq.desktop %buildroot/usr/local/share/applications

mkdir -p %{buildroot}/usr/local/share/pixmaps
cp /usr/local/share/pixmaps/duploq.png %buildroot/usr/local/share/pixmaps


%files
/usr/local/bin/duplo
/usr/local/bin/duploq
/usr/local/share/applications/duploq.desktop
/usr/local/share/pixmaps/duploq.png

%changelog
* Fri Sep 11 2020 Ars Masiuk <ars.masiuk@gmail.com> 0.1
- Initial RPM release

