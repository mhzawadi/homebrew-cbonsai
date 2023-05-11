class Cbonsai < Formula
  desc "Console Bonsai is a bonsai tree generator, written in C using ncurses"
  homepage "https://github.com/mhzawadi/homebrew-cbonsai"
  url "https://github.com/mhzawadi/homebrew-cbonsai/archive/refs/tags/v1.3.5.tar.gz"
  sha256 "265641b6d06bc31927f8bd994fe827bf0a508e047a2e238642d674fd0a614a03"
  license "GPL-3.0"
  depends_on "ncurses"
  depends_on "pkg-config"

  def install
    ENV["PKG_CONFIG_PATH"] = "$PKG_CONFIG_PATH:/usr/local/Cellar/ncurses/6.4/lib/pkgconfig/"
    ENV.append_path "PKG_CONFIG_PATH", "/opt/homebrew/Cellar/ncurses/6.4/lib/pkgconfig/"
    system "make"
    bin.install "cbonsai"
  end

  test do
    system bin/"cbonsai", "-p"
  end
end
