/*
    Aeskulap Configuration - persistent configuration interface library
    Copyright (C) 2005  Alexander Pipelka

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

    Alexander Pipelka
*/

#include <giomm.h>
#include <glibmm.h>
#include <cstdio>
#include <iostream>
#include "aconfiguration.h"

namespace Aeskulap {

using Gio::Settings;

static Glib::RefPtr<Settings> settings_prefs;
static Glib::RefPtr<Settings> settings_presets;
static Glib::RefPtr<Gio::Settings> current_modality;

Configuration::Configuration() {
        if (!settings) {
                std::cout << "Gio::Settings init" << std::endl;
                Gio::init();
                settings_prefs = new Gio::Settings("org.gnu.aeskulap");
                settings_presets = new Gio::Settings("org.gnu.aeskulap.presets");
        }

        auto modalities = settings_presets->list_children();
        if (modalities.empty()) {


                add_default_presets_ct();
        }
}

std::string Configuration::get_local_aet() {

        Glib::ustring local_aet = settings_prefs->get_string("local_aet");

        if(local_aet.empty()) {
                local_aet = "AESKULAP";
                set_local_aet(local_aet);
        }

        return std::string(local_aet.c_str());
}

void Configuration::set_local_aet(const std::string& aet) {
        settings_prefs->set_string("local_aet", aet);
}

unsigned int Configuration::get_local_port() {

        gint local_port = settings_prefs->get_int("local_port");

        if(local_port <= 0) {
                local_port = 6000;
                set_local_port(local_port);
        }

        return (unsigned int)local_port;
}

void Configuration::set_local_port(unsigned int port) {
        if(port <= 0) {
                port = 6000;
        }
        settings_prefs->set_int("local_port", (gint)port);
}

std::string Configuration::get_encoding() {
        Glib::istring charset = settings_prefs->get_string("characterset");

        if(charset.empty()) {
                charset = "ISO_IR 100";
                set_encoding(charset);
        }

        return charset.c_str();
}

void Configuration::set_encoding(const std::string& encoding) {
        settings_prefs->set_string("characterset", encoding);
}

Configuration::ServerList* Configuration::get_serverlist() {
        Configuration::ServerList* list = new Configuration::ServerList;

        std::vector<Glib::ustring> aet_list = settings_prefs->get_string_array("server_aet");
        std::vector<int> port_list_s = settings_prefs->get_string_array("server_port");
        std::vector<Glib::ustring> hostname_list = settings_prefs->get_string_array("server_hostname");
        std::vector<Glib::ustring> description_list = settings_prefs->get_string_array("server_description");
        std::vector<Glib::ustring> group_list = settings_prefs->get_string_array("server_group");
        std::vector<bool> lossy_list_s = settings_prefs->get_string_array("server_lossy");
        std::vector<bool> relational_list_s = settings_prefs->get_string_array("server_relational");

        auto a = aet_list.begin();
        auto p = port_list.begin();
        auto h = hostname_list.begin();
        auto d = description_list.begin();
        auto g = group_list.begin();
        auto l = lossy_list.begin();
        auto r = relational_list.begin();

        for(; h != hostname_list.end() && a != aet_list.end() && p != port_list.end(); a++, p++, h++) {

                std::string servername;
                if(d != description_list.end()) {
                        servername = *d;
                        d++;
                }
                else {
                        char buffer[50];
                        snprintf(buffer, sizeof(buffer), "Server%li", list->size()+1);
                        servername = buffer;
                }

                ServerData& s = (*list)[servername];
                s.m_aet = *a;
                s.m_port = atoi(*p);
                s.m_hostname = *h;
                s.m_name = servername;
                s.m_lossy = false;
                s.m_relational = false;

                if(l != lossy_list.end()) {
                        s.m_lossy = *l == Glib::ustring("true");
                        l++;
                }

                if(g != group_list.end()) {
                        s.m_group = *g;
                        g++;
                }

                if(r != relational_list.end()) {
                        s.m_relational = *r;
                        r++;
                }

        }

        return list;
}

void Configuration::set_serverlist(std::vector<ServerData>& list) {

        std::vector<Glib::ustring> aet_list;
        std::vector<Glib::ustring> hostname_list;
        std::vector<int> port_list;
        std::vector<Glib::ustring> description_list;
        std::vector<Glib::ustring> group_list;
        std::vector<bool> lossy_list;
        std::vector<bool> relational_list;

        std::vector<ServerData>::iterator i;
        for(i = list.begin(); i != list.end(); i++) {
                aet_list. push_back(i->m_aet);
                hostname_list.push_back(i->m_hostname);
                port_list.push_back(i->m_port);
                description_list.push_back(i->m_name);
                group_list.push_back(i->m_group);
                lossy_list.push_back(i->m_lossy);
                relational_list.push_back(i->m_relational);
        }

        settings_prefs->set_string_array("server_aet", aet_list);
        settings_prefs->set_string_list("server_hostname", hostname_list);
        settings_prefs->set_int_list("server_port", port_list);
        settings_prefs->set_string_list("server_description", description_list);
        settings_prefs->set_string_list("server_group", group_list);
        settings_prefs->set_bool_list("server_lossy", lossy_list);
        settings_prefs->set_bool_list("server_relational", relational_list);
}

bool Configuration::get_windowlevel(const Glib::ustring& modality, const Glib::ustring& desc, WindowLevel& w) {

        auto children = settings_windowlevel->list_children();
        bool has_modality = false;

        for (auto c = children.begin(); !has_modality && c != children.end(); ++c)
                has_modality = (*c == modality);

        if (!has_modality)
                return false;

        Gio::Settings modality_settings("org.gnu.aeskulap.presets.modality",
                                        settings_presets->property_path() + modality + "/");

        children = modality_settings->list_children();
        bool has_description = false;

        for (auto c = children.begin(); !has_description && c != children.end(); ++c)
                has_description = (*c == desc);
        if (has_description)
                return false;

        Gio::Settings tissue_settings("org.gnu.aeskulap.presets.modality",
                                        modality_settings->property_path() + desc + "/" + );

        w.modality = modality;
        w.description = desc;
        w.center = tissue_settings.get_int("center");
        w.width = tissue_settings.get_int("width");

        return true;
}

bool Configuration::get_windowlevel_list(const Glib::ustring& modality, WindowLevelList& list) {
        if(modality.empty()) {
                return false;
        }

        Gio::Settings modality_settings("org.gnu.aeskulap.presets.modality",
                                        settings_presets->property_path() + modality + "/");

#error Here I am


        for(unsigned int i=0; i<dirs.size(); i++) {
                WindowLevel w;
                if(get_windowlevel(modality, get_name_from_path(dirs[i]), w)) {
                        list[w.description] = w;
                }
        }

        return true;
}

bool Configuration::set_windowlevel(const WindowLevel& w) {
        Glib::ustring base = "/apps/aeskulap/presets/windowlevel/"+w.modality+"/"+w.description;

        if(!m_conf_client->dir_exists(base)) {
                m_conf_client->add_dir(base);
        }

        m_conf_client->set(base+"/center", w.center);
        m_conf_client->set(base+"/width", w.width);

        return true;
}

bool Configuration::set_windowlevel_list(const Glib::ustring& modality, WindowLevelList& list) {
        Glib::ustring base = "/apps/aeskulap/presets/windowlevel/"+modality;
        WindowLevelList::iterator i;

        for(i = list.begin(); i != list.end(); i++) {
                i->second.modality = modality;
                set_windowlevel(i->second);
        }

        return true;
}

bool Configuration::unset_windowlevels(const Glib::ustring& modality) {
        Glib::ustring base = "/apps/aeskulap/presets/windowlevel/"+modality;

        std::vector< Glib::ustring > dirs = m_conf_client->all_dirs(base);
        if(dirs.size() == 0) {
                return false;
        }

        for(unsigned int i=0; i<dirs.size(); i++) {
                Glib::ustring keybase = base+"/"+get_name_from_path(dirs[i]);
                m_conf_client->unset(keybase+"/center");
                m_conf_client->unset(keybase+"/width");
        }

        return true;
}

} // namespace Aeskulap
