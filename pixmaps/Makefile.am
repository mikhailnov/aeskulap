icondir = $(datadir)/icons/hicolor/64x64/apps
icon_DATA = $(srcdir)/aeskulap.png
svgicondir = $(datadir)/icons/hicolor/scalable/apps
svgicon_DATA = $(srcdir)/aeskulap.svg

gtk_update_icon_cache = gtk-update-icon-cache -f -t $(datadir)/icons/hicolor

install-data-hook:
	@-if test -z "$(DESTDIR)"; then \
		echo "Updating Gtk icon cache."; \
		$(gtk_update_icon_cache); \
	else \
	        echo "*** Icon cache not updated.  After install, run this:"; \
	        echo "***   $(gtk_update_icon_cache)"; \
	fi

imagesdir = $(datadir)/aeskulap/images
images_DATA = \
	$(srcdir)/grid-16.png \
	$(srcdir)/grid-1.png \
	$(srcdir)/grid-2h.png \
	$(srcdir)/grid-2v.png \
	$(srcdir)/grid-4.png \
	$(srcdir)/series-1x1.png \
	$(srcdir)/series-2x1.png \
	$(srcdir)/series-2x2.png \
	$(srcdir)/series-3x2.png \
	$(srcdir)/series-3x3.png \
	$(srcdir)/series-4x4.png \
	$(srcdir)/stock-layers-24.png \
	$(srcdir)/stock-tool-measure-22.png \
	$(srcdir)/stock-tool-scale-22.png \
	$(srcdir)/stock-tool-eraser-22.png \
	$(srcdir)/cursor_pan.png \
	$(srcdir)/cursor_windowlevel.png \
	$(srcdir)/cursor_zoom.png \
	$(srcdir)/filmholes-big-left.png \
	$(srcdir)/filmholes-big-right.png \
	$(srcdir)/start-here.png \
	$(srcdir)/stock-tool-color-picker-22.png

EXTRA_DIST = \
	$(srcdir)/README \
	$(icon_DATA) \
	$(svgicon_DATA) \
	$(images_DATA)
