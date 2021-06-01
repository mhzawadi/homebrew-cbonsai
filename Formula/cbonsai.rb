class Cbonsai < Formula
  desc "Console Bonsai is a bonsai tree generator, written in C using ncurses"
  homepage "https://github.com/mhzawadi/homebrew-cbonsai"
  url "https://github.com/mhzawadi/homebrew-cbonsai/archive/v1.2.0.tar.gz"
  sha256 "a6984df8327141f7e6f24aca0d2194ec14f920ed16a5bc59b4490e9bdde69fba"
  license "GPL-3.0"

  def install
    system "make"
  end

  test do
    system "~/.local/bin/cbonsai", "-p"
  end
end
